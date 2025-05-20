#pragma once

#include "lexer.h"
#include "ast.h"

struct Parser {
    Parser(const char* data) 
     : lexer_(data), cur_token_(lexer_.GetToken()), peek_token_(lexer_.PeekToken())
    {    
        // initialize cur and peek tokens
    }
    void AdvanceTokens() {
        cur_token_ = peek_token_;
        peek_token_ = lexer_.GetToken();
    }
    ptr<Statement> ParseStatement() {
        if (cur_token_.type != TokenType::invalid) {
            if (cur_token_.type == TokenType::if_s) {
                return ParseIfStatement();
            } else if (cur_token_.type == TokenType::ident 
                        && peek_token_.type == TokenType::assign) {
                return ParseAssignStatement();
            }
        } else {
            // pupupu, wrong statement
        }
    }
    
    ptr<AssignStatement> ParseAssignStatement() {
        ptr<AssignStatement> ans = make_ptr<AssignStatement>();
        ans->ident = make_ptr<Identifier>();
        AdvanceTokens();
        ans->expr = ParseExpression();
        return ans;
    }


    ptr<Expression> ParseExpression() {
        ptr<Expression> expr = make_ptr<Expression>();
        // do smth
        return expr;
    }

    template<size_t Level>
    ptr<ExpressionImpl<Level>> ParseLeveledExpression() {
        ptr<ExpressionImpl<Level>> expr = make_ptr<ExpressionImpl<Level>>;
        expr->first = ParseLeveledExpression<Level-1>();
        auto opt_oper = ParseLeveledOperator<Level>();
        while (opt_oper) {
            expr->others.emplace_back(opt_oper, ParseLeveledExpression<Level-1>());
        }
        return expr;
    }

    template<>
    ptr<ExpressionImpl<0>> ParseLeveledExpression() {
        
        if (cur_token_.type == TokenType::func) {
            ptr<FunctionalExpression> ans = make_ptr<FunctionalExpression>();
            ans->func_keyword = cur_token_;
            AdvanceTokens();
            ans->underlying = ParseExpression();
            return ans;
        } else if (IsPrefixOperator(cur_token_.type)) {
            ptr<PrefixExpression> ans = make_ptr<PrefixExpression>();
            ans->prefix_oper = cur_token_;
            AdvanceTokens();
            ans->underlying = ParseLeveledExpression<0>();
            return ans;
        } else if (cur_token_.type == TokenType::lparen) {
            AdvanceTokens();
            ptr<ScopedExpression> ans 
                = make_ptr<ScopedExpression>();
            ans->lparen = cur_token_;
            ans->underlying = ParseExpression();            
            AdvanceTokens();
            // Expect that skiped ')'
            return ans;
        }
    }

    ptr<IfStatement> ParseIfStatement() {
        ptr<IfStatement> if_statement = make_ptr<IfStatement>();
        AdvanceTokens();
        if_statement->condition = ParseExpression();
        if (cur_token_.type != TokenType::then) {
            // missing then
        } else {
            if_statement->underlying = ParseStatement();
            while(true) {
                auto else_if = ParseElseIfs();
                if (else_if.has_value()) {
                    if_statement->else_ifs.push_back(else_if.value());
                } else {
                    break;
                }
            }
            if_statement->else_s = ParseElseStatement();
        }
        return if_statement;
    }
    std::optional<ptr<ElseIfStatement>> ParseElseIfs() {
        if (cur_token_.type == TokenType::else_s) {
            // maybe put in field
            AdvanceTokens();
            ptr<ElseIfStatement> ans = make_ptr<ElseIfStatement>();
            ans->condition = ParseExpression();
            ans->underlying = ParseStatement();
            return ans;
        } else {
            return std::nullopt;
        }
    }
    std::optional<ptr<ElseStatement>> ParseElseStatement() {
        if (cur_token_.type != TokenType::else_s) {
            return std::nullopt;
        }
        ptr<ElseStatement> ans = make_ptr<ElseStatement>();
        ans->token = cur_token_;
        ans->underlying = ParseStatement();
        return ans;
    }
    Lexer lexer_;
    Token cur_token_;
    Token peek_token_;
};