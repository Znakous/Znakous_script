#include "evaluator.h"
#include "std_functions.h"

double stoi_view(std::string_view s) {
    int ans = 0;
    if (std::from_chars(s.data(), s.data() + s.size(), ans).ec
            == std::errc::invalid_argument) {
        throw std::invalid_argument("invalid stoi_view argument");
    } 
    return ans;
}


Evaluator::Evaluator(std::ostream& out) : out_(out) {}

Stopping Evaluator::operator()(ptr<Statement>& statement) {
    std::cout << "statemnet\n";
    return std::visit(*this, *statement);
    // return ret;
}
Stopping Evaluator::operator()(ptr<IfStatement>& expr) {
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
    // return ret;
}
Stopping Evaluator::operator()(ptr<AssignStatement>& expr) {
    std::cout << "we assign!!!!!!!\n";
    env_.Declare(expr->ident);
    std::cout << "declared\n";
    auto rhs = std::visit(*this, expr->expr);
    std::cout << "got rhs\n";
    std::cout << "ident: " << expr->ident << "\n";
    std::cout << typeid(rhs).name();
    env_[expr->ident] = rhs;
    std::cout << "we assigned\n";
    return Stopping::none;
}
Stopping Evaluator::operator()(ptr<ReturnStatement>& ret_st) {
    std::cout << "ret state\n";
    return_object_ = std::visit(*this, ret_st->value);
    if (auto func = std::get_if<ptr<FunctionalExpression>>(&return_object_.value())) {
        if (!(*func)->closure_env) {
            (*func)->closure_env = make_ptr<Environment>(*env_.current);
        }
    }
    return Stopping::return_s;
    // auto ret = make_ptr<Object>();
    // *ret = std::string("aaaaaa");
    // return ret;
    // return ret;
}

Stopping Evaluator::operator()(ptr<ExprStatement>& expr) {
    std::visit(*this, expr->expr);
    return Stopping::none;
}

Stopping Evaluator::operator()(ptr<WhileStatement>& expr) {
    std::cout << "while statement\n";
    while (std::visit(TruthChecker{}, std::visit(*this, expr->condition))) {
        std::cout << "while body\n";
        for (decltype(auto) el : expr->body) {
            auto stop_res = std::visit(*this, el);
            if (stop_res == Stopping::break_s) {
                break;
            } else if (stop_res == Stopping::return_s) {
                return Stopping::return_s;
            } else if (stop_res == Stopping::continue_s) {
                continue;
            }
        }
    }
    return Stopping::none;
}
Object Evaluator::operator()(ptr<IntLiteralExpression>& expr) {
    std::cout << "int literal\n";
    return stoi_view(expr->literal.value.value());
}
Object Evaluator::operator()(ptr<StringLiteralExpression>& expr) {
    std::cout << "string literal\n";
    return std::string(expr->literal.value.value());
}

Object Evaluator::ExecStd(ptr<FunctionCallExpression>& expr) {
    if (expr->function == "print") {
        return std::visit(Print{out_}, std::visit(*this, expr->arguments[0]));
    }
}
Object Evaluator::operator()(ptr<FunctionCallExpression>& expr) {
    if (expr->token.type == TokenType::stdfunc) {
        return ExecStd(expr);
    }
    std::cout << "func call literal\n";
    auto function = (env_[expr->function]);
    std::cout << "hui\n";

    std::cout << function.index() << " <----\n";
    ptr<FunctionalExpression> to_invoke
         = std::get<ptr<FunctionalExpression>>
         (function);
    std::cout << "know what to invoke\n";
    
    env_.Enclose();
    
    if (to_invoke->closure_env) {
        env_.current->CopyFrom(*to_invoke->closure_env);
    }
    
    for (int i = 0; i < to_invoke->arguments.size(); ++i) {
        std::cout << "get val for " << to_invoke->arguments[i] << "\n";
        env_.Declare(to_invoke->arguments[i]);
        env_[to_invoke->arguments[i]] = std::visit(*this, expr->arguments[i]);
    }
    
    for (decltype(auto) st : to_invoke->body) {
        Stopping stop_res = std::visit(*this, st);
        if (stop_res == Stopping::return_s) {
            std::cout << "      stopped\n";
            env_.OutClose();
            return return_object_.value();
        }
    }
    
    std::cout << "func did't return !!!!!!!!!!!!!!!\n";
    env_.OutClose();
    return CNull();
}
Object Evaluator::operator()(ptr<IdentifierExpression>& expr) {
    std::cout << "identifier\n";
    auto ans = env_.GetByIdent(expr->name).value();
    std::cout << "parsed from ident\n";
    return ans;
}

Object Evaluator::operator()(ptr<FunctionalExpression>& expr) {
    expr->closure_env = make_ptr<Environment>(*env_.current);
    return expr;
}
Object Evaluator::operator()(ptr<ScopedExpression>& expr) {
    std::cout << "scoped\n";
    return std::visit(*this, expr->underlying);
}
// Object visit(PrefixExpression& expr) {
//     if (expr.prefix_oper == TokenType::minus) {
//         return 
//     }
// }
