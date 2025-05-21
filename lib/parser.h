#pragma once

#include "lexer.h"
#include "ast.h"

struct Parser {
    Parser(const char* data);
    void AdvanceTokens();
    ptr<Statement> ParseStatement();
    
    ptr<AssignStatement> ParseAssignStatement();

    ptr<Program> ParseProgram();

    ptr<Expression> ParseExpression();

    template<size_t Level>
    ptr<ExpressionImpl<Level>> ParseLeveledExpression() {
        ptr<ExpressionImpl<Level>> expr = make_ptr<ExpressionImpl<Level>>;
        expr->first = ParseLeveledExpression<Level-1>();
        while(true) {
            if (GetLevel(cur_token_) == Level) {
                expr->others.emplace_back(cur_token_, ParseLeveledExpression<Level-1>());
            } else {
                break;
            }
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

    ptr<IfStatement> ParseIfStatement();

    std::optional<ptr<ElseIfStatement>> ParseElseIfs();
    std::optional<ptr<ElseStatement>> ParseElseStatement();

    Lexer lexer_;
    Token cur_token_;
    Token peek_token_;
};