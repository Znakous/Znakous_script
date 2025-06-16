#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <charconv>
#include <stdexcept>
#include <exception>

#include "usings.h"
#include "trie.h"
#include "environment.h"
#include "operators_executor.h"
#include "object.h"
#include "ast/ast.h"
#include "logger.h"

double stoi_view(std::string_view);

enum class Stopping {
    return_s, break_s, continue_s, none
};

struct Evaluator {
    Evaluator(std::ostream& out, std::shared_ptr<logging::Logger> log);

    template<size_t Level>
    Object operator()(ptr<ExpressionImpl<Level>>& expr) {
        logger_->log("Evaluating expression at level " + std::to_string(Level));
        Object result = (*this)(expr->first);
        logger_->log("First operand evaluated");
        
        for(auto op_and_expr : expr->others) {
            logger_->log("Processing operator: " + std::to_string((int)op_and_expr.first.token.type));
            Object from_rhs = (*this)(op_and_expr.second);
            Object lhs = result;
            Object rhs = from_rhs;
            result = exec_bin_.Execute(op_and_expr.first.token.type, lhs, rhs);
            logger_->log("Operation completed");
        }
        return result;
    }

    template<>
    Object operator()(ptr<ExpressionImpl<1>>& expr) {
        logger_->log("Evaluating level 1 expression");
        Object result = (*this)(expr->underlying);
        
        if (expr->prefix_oper) {
            logger_->log("Applying prefix operator: " + std::to_string((int)expr->prefix_oper.value().type));
            Object operand = result;
            result = exec_un_.Execute(expr->prefix_oper.value().type, operand);
        }
        return result;
    }

    template<>
    Object operator()(ptr<ExpressionImpl<0>>& expr) {
        logger_->log("Evaluating level 0 expression");
        return std::visit(*this, expr->value);
    }

    Stopping operator()(ptr<Statement>& expr);
    Stopping operator()(ptr<IfStatement>& expr);
    Stopping operator()(ptr<AssignStatement>& expr);
    Stopping operator()(ptr<ReturnStatement>& expr);
    Stopping operator()(ptr<WhileStatement>& expr);
    Stopping operator()(ptr<ExprStatement>& expr);
    Stopping operator()(ptr<ArrayAssignStatement>& expr);
    Stopping operator()(ptr<BreakStatement>&) { 
        logger_->log("Evaluating break statement");
        return Stopping::break_s; 
    }
    Stopping operator()(ptr<ContinueStatement>&) { 
        logger_->log("Evaluating continue statement");
        return Stopping::continue_s; 
    }

    Object operator()(ptr<IntLiteralExpression>& expr);
    Object operator()(ptr<StringLiteralExpression>& expr);
    Object operator()(ptr<FunctionalExpression>& expr);
    Object operator()(ptr<FunctionCallExpression>& expr);
    Object operator()(ptr<IdentifierExpression>& expr);
    Object operator()(ptr<ScopedExpression>& expr);
    Object operator()(ptr<ArrayExpression>& expr);
    Object operator()(ptr<ArrayAccessExpression>& expr);
    Object operator()(ptr<ArraySliceExpression>& expr);
    Object operator()(ptr<StdFuncCallExpression>& expr);
private:
    Object ExecStd(ptr<StdFuncCallExpression>& expr);
    std::pair<CArray*, size_t> traverse_array(CArray& root_array, std::vector<Expression>& indices);
    std::optional<Object> return_object_;
    std::reference_wrapper<std::ostream> out_;
    EnvironmentMaster env_;
    BinaryOperatorExecutor exec_bin_;
    UnaryOperatorExecutor exec_un_;
    std::shared_ptr<logging::Logger> logger_;
};