#pragma once

#include "usings.h"
#include "lexer.h"
#include "ast/ast.h"

struct Parser {
    Parser(const char* data);
    void AdvanceTokens();
    Statement ParseStatement();
    
    Statement ParseAssignStatement();

    ptr<Program> ParseProgram();

    ptr<ExprStatement> ParseExprStatement();
    ptr<WhileStatement> ParseWhileStatement();

    Expression ParseExpression();

    template<size_t Level>
    ptr<ExpressionImpl<Level>> ParseLeveledExpression() {
        std::cout << "parsing leveled " << Level << "\n";

        ptr<ExpressionImpl<Level>> expr = make_ptr<ExpressionImpl<Level>>();
        expr->first = ParseLeveledExpression<Level-1>();
        while(true) {
            if (IsLevelOperator<Level>(cur_token_)) {
                auto oper = cur_token_;
                AdvanceTokens();
                expr->others.emplace_back(oper, ParseLeveledExpression<Level-1>());
            } else {
                break;
            }
        }
        return expr;
    }


    template<>
    ptr<ExpressionImpl<1>> ParseLeveledExpression() {
        std::cout << "parsing leveled " << 1 << "\n";

        ptr<ExpressionImpl<1>> ans = make_ptr<ExpressionImpl<1>>();
        if (IsPrefixOperator(cur_token_.type)) {
            std::cout << "ddddf is that\n";
            ans->prefix_oper = cur_token_;
            AdvanceTokens();
            ans->underlying = ParseLeveledExpression<0>();
            return ans;
        }
        else {
            ans->prefix_oper = std::nullopt;
            // AdvanceTokens();
            ans->underlying = ParseLeveledExpression<0>();
            return ans;
        }
    }
    template<>
    ptr<ExpressionImpl<0>> ParseLeveledExpression() {
        std::cout << "parsing level 0\n"; 
        std::cout << "token " << (int) cur_token_.type << "\n";
        ptr<ExpressionImpl<0>> ans = make_ptr<ExpressionImpl<0>>();
        if (cur_token_.type == TokenType::lparen) {
            std::cout << "lparen seen\n"; 
            AdvanceTokens();
            ptr<ScopedExpression> scoped 
                = make_ptr<ScopedExpression>();
            scoped->underlying = ParseExpression();            
            AdvanceTokens();
            // AdvanceTokens();
            ans->value = scoped;
            // Expect that skiped ')'
            // return ans;
        } else if (cur_token_.type == TokenType::intliteral) {
            std::cout << "it's number!!\n";
            auto lit = make_ptr<IntLiteralExpression>();
            lit->literal = cur_token_;
            std::cout << "number is " << lit->literal.value.value() << " " << lit->literal.value.value().size() << "\n";
            AdvanceTokens();
            ans->value = lit;
            // return ans;
        } else if (cur_token_.type == TokenType::stringliteral) {
            std::cout << "string literal\n";
            auto lit = make_ptr<StringLiteralExpression>();
            lit->literal = cur_token_;
            AdvanceTokens();
            ans->value = lit;
            // return ans;
        }else if (cur_token_.type == TokenType::lsquare) {
            // Parse array literal
            std::cout << "array literal\n";
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
        } else if (cur_token_.type == TokenType::ident && peek_token_.type == TokenType::lsquare) {
            std::cout << "array access parse\n";
            auto access = make_ptr<ArrayAccessExpression>();
            access->array = cur_token_.value.value();
            AdvanceTokens(); // skip name
            
            while (cur_token_.type == TokenType::lsquare) {
                AdvanceTokens(); // skip [
                access->indices.push_back(ParseExpression());
                AdvanceTokens(); // skip ]
            }
            ans->value = access;
        }
        else if ((cur_token_.type == TokenType::ident || cur_token_.type == TokenType::stdfunc) && peek_token_.type == TokenType::lparen) {
            std::cout << "i make func call\n";
            auto call = make_ptr<FunctionCallExpression>();
            call->function = cur_token_.value.value();
            call->token = cur_token_;
            AdvanceTokens();
            call->arguments = ParseArguments();
            // while (cur_token_.type != TokenType::rparen) {
            //     call->arguments.push_back(ParseExpression());
            //     // if (cur_token_.type == TokenType::comma) AdvanceTokens();
            // }
            // AdvanceTokens();
            ans->value = call;
            std::cout << "i parsedfunc call\n";
            // return ans;
        } 
        else if (cur_token_.type == TokenType::ident) {
            std::cout << "it's ident\n";
            auto ident = make_ptr<IdentifierExpression>();
            ident->name = cur_token_.value.value();
            AdvanceTokens();
            ans->value = ident;
            // return ans;
        }
        std::cout << "wdf is that\n";
        std::cout << (int) cur_token_.type;
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
};