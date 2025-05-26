#pragma once

#include "usings.h"
#include "lexer.h"
#include "ast/ast.h"

struct Parser {
    Parser(const char* data);
    void AdvanceTokens();
    ptr<Statement> ParseStatement();
    
    ptr<AssignStatement> ParseAssignStatement();

    ptr<Program> ParseProgram();

    ptr<Expression> ParseExpression();

    template<size_t Level>
    ptr<ExpressionImpl<Level>> ParseLeveledExpression() {
        ptr<ExpressionImpl<Level>> expr = make_ptr<ExpressionImpl<Level>>();
        expr->first = ParseLeveledExpression<Level-1>();
        while(true) {
            if (IsLevelOperator<Level>(cur_token_)) {
                expr->others.emplace_back(cur_token_, ParseLeveledExpression<Level-1>());
            } else {
                break;
            }
        }
        return expr;
    }


    template<>
    ptr<ExpressionImpl<1>> ParseLeveledExpression() {
        ptr<ExpressionImpl<1>> ans = make_ptr<ExpressionImpl<1>>();
        if (IsPrefixOperator(cur_token_.type)) {
            ans->prefix_oper = cur_token_;
            AdvanceTokens();
            ans->underlying = ParseLeveledExpression<0>();
            return ans;
        }
        else {
            ans->prefix_oper = std::nullopt;
            AdvanceTokens();
            ans->underlying = ParseLeveledExpression<0>();
            return ans;
        }
    }
    template<>
    ptr<ExpressionImpl<0>> ParseLeveledExpression() {
        ptr<ExpressionImpl<0>> ans = make_ptr<ExpressionImpl<0>>();
        if (cur_token_.type == TokenType::lparen) {
            AdvanceTokens();
            ptr<ScopedExpression> scoped 
                = make_ptr<ScopedExpression>();
            scoped->underlying = ParseExpression();            
            AdvanceTokens();
            ans->value = scoped;
            // Expect that skiped ')'
            // return ans;
        } else if (cur_token_.type == TokenType::number) {
            auto lit = make_ptr<IntLiteralExpression>();
            lit->literal = cur_token_;
            AdvanceTokens();
            ans->value = lit;
            // return ans;
        } else if (cur_token_.type == TokenType::stringliteral) {
            auto lit = make_ptr<StringLiteralExpression>();
            lit->literal = cur_token_;
            AdvanceTokens();
            ans->value = lit;
            // return ans;
        } else if (cur_token_.type == TokenType::ident) {
            if (peek_token_.type == TokenType::lparen) {
                auto call = make_ptr<FunctionCallExpression>();
                call->function = cur_token_.value.value();
                AdvanceTokens();
                call->arguments = ParseArguments();
                // while (cur_token_.type != TokenType::rparen) {
                //     call->arguments.push_back(ParseExpression());
                //     // if (cur_token_.type == TokenType::comma) AdvanceTokens();
                // }
                AdvanceTokens();
                ans->value = call;
                // return ans;
            } else {
                auto ident = make_ptr<IdentifierExpression>();
                ident->name = cur_token_.value.value();
                AdvanceTokens();
                ans->value = ident;
                // return ans;
            }
        }
        return ans;
    }

    ptr<Expression> ParseReturnStatement();
    ptr<IfStatement> ParseIfStatement();

    std::optional<ptr<ElseIfStatement>> ParseElseIfs();
    std::optional<ptr<ElseStatement>> ParseElseStatement();
private:
    std::vector<ptr<IdentifierExpression>> ParseArguments();
    Lexer lexer_;
    Token cur_token_;
    Token peek_token_;
};