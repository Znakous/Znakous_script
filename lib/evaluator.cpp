#include "evaluator.h"
#include "std_functions.h"
#include "logger.h"

double stoi_view(std::string_view s) {
    // int ans = 0;
    // if (std::from_chars(s.data(), s.data() + s.size(), ans).ec
    //         == std::errc::invalid_argument) {
    //     throw std::invalid_argument("invalid stoi_view argument");
    // } 
    // std::cout << "stoi_view: " <<  << " " << s << "\n";
    // throw std::runtime_error("stoi_view: " + std::to_string(ans));
    return std::stod(s.data(), nullptr);
}


Evaluator::Evaluator(std::ostream& out, std::shared_ptr<logging::Logger> log)
 : out_(out), logger_(log), exec_bin_(log), exec_un_(log), env_(log)
{}

Stopping Evaluator::operator()(ptr<Statement>& statement) {
    logger_->log("Evaluating statement");
    return std::visit(*this, *statement);
}
Stopping Evaluator::operator()(ptr<IfStatement>& expr) {
    logger_->log("Evaluating if statement");
    auto eval_res = std::visit(*this, expr->condition);
    if (std::visit(TruthChecker{}, eval_res)) {
        env_.Enclose();
        auto ans = std::visit(*this, expr->to_do);
        env_.OutClose();
        return ans;
    }
    for (auto& el : expr->else_ifs) {
        auto eval_eif_res = std::visit(*this, el->condition);
        if (std::visit(TruthChecker{}, eval_eif_res)) {
            return std::visit(*this, el->underlying);
        }
    }
    if (expr->else_s.has_value()) {
        return std::visit(*this, expr->else_s.value()->underlying);
    }
    return Stopping::none;
}
Stopping Evaluator::operator()(ptr<AssignStatement>& expr) {
    logger_->log("Processing assignment");
    env_.Declare(expr->ident);
    auto rhs = std::visit(*this, expr->expr);
    logger_->log("Assigning to identifier: ", expr->ident);
    env_[expr->ident] = rhs;
    return Stopping::none;
}
Stopping Evaluator::operator()(ptr<ReturnStatement>& ret_st) {
    logger_->log("Processing return statement");
    return_object_ = std::visit(*this, ret_st->value);
    if (auto func = std::get_if<ptr<FunctionalExpression>>(&return_object_.value())) {
        if (!(*func)->closure_env) {
            (*func)->closure_env = make_ptr<Environment>(*env_.current);
        }
    }
    return Stopping::return_s;
}

Stopping Evaluator::operator()(ptr<ExprStatement>& expr) {
    logger_->log("Evaluating expression statement");
    std::visit(*this, expr->expr);
    return Stopping::none;
}

Stopping Evaluator::operator()(ptr<WhileStatement>& expr) {
    logger_->log("Processing while statement");
    env_.Enclose();  // Create environment once before the loop
    while (std::visit(TruthChecker{}, std::visit(*this, expr->condition))) {
        logger_->log("Executing while body");
        for (decltype(auto) el : expr->body) {
            auto stop_res = std::visit(*this, el);
            if (stop_res == Stopping::break_s) {
                env_.OutClose();
                return Stopping::none;  // Break out of the loop
            } else if (stop_res == Stopping::return_s) {
                env_.OutClose();
                return Stopping::return_s;  // Return from function
            } else if (stop_res == Stopping::continue_s) {
                break;  // Continue to next iteration
            }
        }
    }
    env_.OutClose();
    return Stopping::none;
}
Object Evaluator::operator()(ptr<IntLiteralExpression>& expr) {
    logger_->log("Evaluating integer literal");
    return stoi_view(expr->literal.value.value());
}
Object Evaluator::operator()(ptr<StringLiteralExpression>& expr) {
    logger_->log("Evaluating string literal");
    return std::string(expr->literal.value.value());
}

Object Evaluator::operator()(ptr<StdFuncCallExpression>& expr) {
    std::vector<Object> args;
    for (auto& arg : expr->arguments) {
        args.push_back(std::visit(*this, arg));
    }   
    auto to_exec = GetStdFunc(expr->function, out_, std::move(args));
    return std::visit(Executor{}, *to_exec);
}
Object Evaluator::operator()(ptr<FunctionCallExpression>& expr) {
    logger_->log("Executing function call: ", expr);
    if (std::holds_alternative<ptr<FunctionCallExpression>>(expr->function->value)) {
        std::cout << std::get<ptr<FunctionCallExpression>>(expr->function->value);
        throw std::runtime_error("Function call is smae");
    }
    auto function = this->operator()(expr->function);


    ptr<FunctionalExpression> to_invoke = std::get<ptr<FunctionalExpression>>(function);
    logger_->log("Function resolved, preparing to invoke");
    
    env_.Enclose();
    
    if (to_invoke->closure_env) {
        env_.current->CopyFrom(*to_invoke->closure_env);
    }
    
    for (int i = 0; i < to_invoke->arguments.size(); ++i) {
        logger_->log("Processing argument: ", to_invoke->arguments[i]);
        env_.Declare(to_invoke->arguments[i]);
        env_[to_invoke->arguments[i]] = std::visit(*this, expr->arguments[i]);
    }
    
    for (decltype(auto) st : to_invoke->body) {
        Stopping stop_res = std::visit(*this, st);
        if (stop_res == Stopping::return_s) {
            logger_->log("Function returned");
            env_.OutClose();
            return return_object_.value();
        }
    }
    
    logger_->log("Function completed without return");
    env_.OutClose();
    return CNull();
}
Object Evaluator::operator()(ptr<IdentifierExpression>& expr) {
    logger_->log("Evaluating identifier: ", expr->name);
    return env_.GetByIdent(expr->name).value();
}

Object Evaluator::operator()(ptr<FunctionalExpression>& expr) {
    logger_->log("Creating function closure");
    expr->closure_env = make_ptr<Environment>(*env_.current);
    return expr;
}
Object Evaluator::operator()(ptr<ScopedExpression>& expr) {
    logger_->log("Evaluating scoped expression");
    return std::visit(*this, expr->underlying);
}
// Object visit(PrefixExpression& expr) {
//     if (expr.prefix_oper == TokenType::minus) {
//         return 
//     }
// }

Object Evaluator::operator()(ptr<ArrayExpression>& expr) {
    logger_->log("Creating array");
    CArray array;
    for (auto& element : expr->elements) {
        array.arr.push_back(std::visit(*this, element));
    }
    return array;
}

std::pair<CArray*, size_t> Evaluator::traverse_array(CArray& root_array, std::vector<Expression>& indices) {
    if (indices.empty()) {
        throw std::runtime_error("No indices provided for array access");
    }
    
    CArray* current_array = &root_array;
    for (size_t i = 0; i < indices.size() - 1; ++i) {
        Object index_obj = std::visit(*this, indices[i]);
        size_t idx = std::get<double>(index_obj);
        if (idx >= current_array->arr.size()) {
            throw std::runtime_error("Array index out of bounds");
        }
        if (!std::holds_alternative<CArray>(current_array->arr[idx])) {
            throw std::runtime_error("Attempting to index a non-array element");
        }
        current_array = &std::get<CArray>(current_array->arr[idx]);
    }
    
    Object final_index_obj = std::visit(*this, indices.back());
    size_t final_idx = std::get<double>(final_index_obj);
    if (final_idx >= current_array->arr.size()) {
        throw std::runtime_error("Array index out of bounds");
    }
    
    return {current_array, final_idx};
}

Object Evaluator::operator()(ptr<ArrayAccessExpression>& expr) {
    logger_->log("Accessing array");
    auto arr = this->operator()(expr->array);
    CArray& array = std::get<CArray>(arr);
    
    auto [target_array, idx] = traverse_array(array, expr->indices);
    return target_array->arr[idx];
}

Stopping Evaluator::operator()(ptr<ArrayAssignStatement>& expr) {
    logger_->log("Assigning to array");
   
    auto& arr = env_[expr->array];
    CArray& array = std::get<CArray>(arr);
    
    Object value = std::visit(*this, expr->value);
    
    if (expr->indices.size() == 1) {
        Object index_obj = std::visit(*this, expr->indices[0]);
        size_t idx = std::get<double>(index_obj);
        if (idx >= array.arr.size()) {
            array.arr.resize(idx + 1);
        }
        array.arr[idx] = value;
        return Stopping::none;
    }
    
    CArray* current_array = &array;
    for (size_t i = 0; i < expr->indices.size() - 1; ++i) {
        Object index_obj = std::visit(*this, expr->indices[i]);
        size_t idx = std::get<double>(index_obj);
        if (idx >= current_array->arr.size()) {
            current_array->arr.resize(idx + 1);
        }
        if (!std::holds_alternative<CArray>(current_array->arr[idx])) {
            current_array->arr[idx] = CArray{};
        }
        current_array = &std::get<CArray>(current_array->arr[idx]);
    }
    
    Object final_index_obj = std::visit(*this, expr->indices.back());
    size_t final_idx = std::get<double>(final_index_obj);
    if (final_idx >= current_array->arr.size()) {
        current_array->arr.resize(final_idx + 1);
    }
    current_array->arr[final_idx] = value;
    
    return Stopping::none;
}
