#pragma once

#include "usings.h"
#include "lexer.h"
#include "ast/ast.h"
#include "logger.h"

struct Parser {
    Parser(const char* data, std::shared_ptr<logging::Logger> log);
    Parser(std::istream& in, std::shared_ptr<logging::Logger> log);
    void AdvanceTokens();
    Statement ParseStatement();
    
    Statement ParseAssignStatement();

    ptr<Program> ParseProgram();

    ptr<ExprStatement> ParseExprStatement();
    ptr<WhileStatement> ParseWhileStatement();

    Expression ParseExpression();

    template<size_t Level>
    ptr<ExpressionImpl<Level>> ParseLeveledExpression() {
        logger_->log("Parsing expression at level ", Level);

        ptr<ExpressionImpl<Level>> expr = make_ptr<ExpressionImpl<Level>>();
        expr->first = ParseLeveledExpression<Level-1>();
        while(IsLevelOperator<Level>(cur_token_)) {
            auto oper = cur_token_;
            AdvanceTokens();
            auto rhs = ParseLeveledExpression<Level-1>();
            expr->others.emplace_back(Operator<Level>{oper}, rhs);
        }
        return expr;
    }

    template<>
    ptr<ExpressionImpl<1>> ParseLeveledExpression() {
        logger_->log("Parsing expression at level 1");

        ptr<ExpressionImpl<1>> ans = make_ptr<ExpressionImpl<1>>();
        if (IsPrefixOperator(cur_token_.type)) {
            logger_->log("Found prefix operator");
            ans->prefix_oper = cur_token_;
            AdvanceTokens();
            ans->underlying = ParseLeveledExpression<0>();
        } else {
            ans->prefix_oper = std::nullopt;
            ans->underlying = ParseLeveledExpression<0>();
        }
        return ans;
    }

    template<>
    ptr<ExpressionImpl<0>> ParseLeveledExpression() {
        logger_->log("Parsing expression at level 0, token type: ", static_cast<int>(cur_token_.type));

        ptr<ExpressionImpl<0>> ans = make_ptr<ExpressionImpl<0>>();
        if (cur_token_.type == TokenType::lparen) {
            logger_->log("Parsing parenthesized expression");
            AdvanceTokens();
            ptr<ScopedExpression> scoped = make_ptr<ScopedExpression>();
            scoped->underlying = ParseExpression();            
            AdvanceTokens();
            ans->value = scoped;
        } else if (cur_token_.type == TokenType::intliteral) {
            logger_->log("Parsing integer literal: ", cur_token_.value.value());
            auto lit = make_ptr<IntLiteralExpression>();
            lit->literal = cur_token_;
            AdvanceTokens();
            ans->value = lit;
        } else if (cur_token_.type == TokenType::stringliteral) {
            logger_->log("Parsing string literal");
            auto lit = make_ptr<StringLiteralExpression>();
            lit->literal = cur_token_;
            AdvanceTokens();
            ans->value = lit;
        } else if (cur_token_.type == TokenType::lsquare) {
            logger_->log("Parsing array literal");
            ptr<ArrayExpression> array = make_ptr<ArrayExpression>();
            AdvanceTokens(); // skip [
            
            while (cur_token_.type != TokenType::rsquare) {
                array->elements.push_back(ParseExpression());
                if (cur_token_.type == TokenType::comma) {
                    AdvanceTokens();
                }
            }
            AdvanceTokens(); // skip ]
            ans->value = array;
        }
        else if (cur_token_.type == TokenType::ident) {
            logger_->log("Parsing identifier: ", cur_token_.value.value());
            std::string_view name = cur_token_.value.value();
            AdvanceTokens();
            
            // Create initial identifier expression
            ptr<ExpressionImpl<0>> expr = make_ptr<ExpressionImpl<0>>();
            auto ident = make_ptr<IdentifierExpression>();
            ident->name = name;
            expr->value = ident;
            
            // Handle function calls and array access
            while (cur_token_.type == TokenType::lparen || cur_token_.type == TokenType::lsquare) {
                if (cur_token_.type == TokenType::lparen) {
                    logger_->log("Parsing function call");
                    auto call = make_ptr<FunctionCallExpression>();
                    call->token = cur_token_;
                    
                    // Create a new expression to hold the previous value
                    ptr<ExpressionImpl<0>> func_expr = make_ptr<ExpressionImpl<0>>();
                    func_expr->value = expr->value;  // Copy the previous value
                    call->function = func_expr;      // Use the new expression
                    
                    call->arguments = ParseArguments();
                    
                    // Update expr with the new function call
                    expr = make_ptr<ExpressionImpl<0>>();
                    expr->value = call;
                } else if (cur_token_.type == TokenType::lsquare) {
                    logger_->log("Parsing array access");
                    auto access = make_ptr<ArrayAccessExpression>();
                    
                    // Create a new expression to hold the previous value
                    ptr<ExpressionImpl<0>> array_expr = make_ptr<ExpressionImpl<0>>();
                    array_expr->value = expr->value;  // Copy the previous value
                    access->array = array_expr;       // Use the new expression
                    
                    AdvanceTokens(); // skip [
                    access->indices.push_back(ParseExpression());
                    AdvanceTokens(); // skip ]
                    
                    // Update expr with the new array access
                    expr = make_ptr<ExpressionImpl<0>>();
                    expr->value = access;
                }
            }
            ans = expr;
        }
        else if (cur_token_.type == TokenType::stdfunc && peek_token_.type == TokenType::lparen) {
            logger_->log("Parsing std function call");
            auto call = make_ptr<StdFuncCallExpression>();
            call->token = cur_token_;
            call->function = cur_token_.value.value();
            AdvanceTokens();
            call->arguments = ParseArguments();
            ans->value = call;
        }
        return ans;
    }

    ptr<ReturnStatement> ParseReturnStatement();
    ptr<IfStatement> ParseIfStatement();

    std::optional<ptr<ElseIfStatement>> ParseElseIfs();
    std::optional<ptr<ElseStatement>> ParseElseStatement();
private:
    std::vector<Expression> ParseArguments();
    Lexer lexer_;
    Token cur_token_;
    Token peek_token_;
    std::shared_ptr<logging::Logger> logger_;
};