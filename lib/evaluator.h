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

double stoi_view(std::string_view);

// struct ExprVisitor{
//     ~ExprVisitor() = default;
//     Object visit(IntLiteralExpression& expr) {

//     }
//     Object visit(StringLiteralExpression& expr) = 0;
//     Object visit(IdentifierExpression& expr) = 0;
//     Object visit(PrefixExpression& expr) = 0;
// };

enum class Stopping {
    return_s, break_s, continue_s, none
};

struct Evaluator {
    Evaluator(std::ostream&);
    template<size_t Level>
    Object operator()(ptr<ExpressionImpl<Level>>& expr) {
        std::cout << "eval expr impl\n";
        Object result = (*this)(expr->first);
        for(auto op_and_expr : expr->others) {
            std::cout << "in for\n";
            Object from_rhs = (*this)(op_and_expr.second);
            std::cout << "taken\n";
            result = exec_bin_.Execute(op_and_expr.first.token.type, result, from_rhs);
        }
        std::cout << "eval expr impl ret\n";
        return result;
    }

    template<>
    Object operator()(ptr<ExpressionImpl<1>>& expr) {
        std::cout << "lev 1\n";
        Object result = (*this)(expr->underlying);
        if (expr->prefix_oper) {
            std::cout << "pref_ex\n";
            return exec_un_.Execute(expr->prefix_oper.value().type, result);
        }
        std::cout << "lev 1 ret\n";
        return result;
    }
    template<>
    Object operator()(ptr<ExpressionImpl<0>>& expr) {
        std::cout << "lev 0\n";

        return std::visit(*this, expr->value);
    }


    Stopping operator()(ptr<Statement>& expr);
    Stopping operator()(ptr<IfStatement>& expr);
    Stopping operator()(ptr<AssignStatement>& expr);
    Stopping operator()(ptr<ReturnStatement>& expr);

    Object operator()(ptr<IntLiteralExpression>& expr);
    Object operator()(ptr<StringLiteralExpression>& expr);

    Object operator()(ptr<FunctionalExpression>& expr);
    Object operator()(ptr<FunctionCallExpression>& expr);
    Object operator()(ptr<IdentifierExpression>& expr);
    Object operator()(ptr<ScopedExpression>& expr);
    // Object visit(PrefixExpression& expr) {
    //     if (expr.prefix_oper == TokenType::minus) {
    //         return 
    //     }
    // }
private:
    Object ExecStd(ptr<FunctionCallExpression>& expr);
    std::optional<Object> return_object_;
    std::reference_wrapper<std::ostream> out_;
    EnvironmentMaster env_;
    BinaryOperatorExecutor exec_bin_;
    UnaryOperatorExecutor exec_un_;
};