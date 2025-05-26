#include "parser.h"

Parser::Parser(const char* data) 
    : lexer_(data), cur_token_(lexer_.GetToken()), peek_token_(lexer_.PeekToken())
{    
    // initialize cur and peek tokens
}
void Parser::AdvanceTokens() {
    cur_token_ = peek_token_;
    peek_token_ = lexer_.GetToken();
}
ptr<Statement> Parser::ParseStatement() {
    if (cur_token_.type != TokenType::invalid) {
        if (cur_token_.type == TokenType::if_s) {
            return make_ptr<Statement>(ParseIfStatement());
        } else if (cur_token_.type == TokenType::ident 
                    && peek_token_.type == TokenType::assign) {
            return make_ptr<Statement>(ParseAssignStatement());
        }
    } else {
        // pupupu, wrong statement
    }
}

ptr<AssignStatement> Parser::ParseAssignStatement() {
    ptr<AssignStatement> ans = make_ptr<AssignStatement>();
    ans->ident = make_ptr<IdentifierExpression>();
    AdvanceTokens();
    ans->expr = ParseExpression();
    return ans;
}

ptr<Program> Parser::ParseProgram() {
    ptr<Program> ans = make_ptr<Program>();
    if (cur_token_.type != TokenType::invalid) {
        ans->statements.emplace_back(ParseStatement());
    }
    return ans;
}

ptr<Expression> Parser::ParseExpression() {
    ptr<Expression> expr = make_ptr<Expression>();
    if (cur_token_.type == TokenType::func) {
        AdvanceTokens(); // skip func
        ptr<FunctionalExpression> ans = make_ptr<FunctionalExpression>();
        while (cur_token_.type != TokenType::ret) {
            ans->body.push_back(ParseStatement());
        }
        ans->return_s = ParseReturnStatement();
        *expr = ans;
    } else {
        *expr = ParseLeveledExpression<operators_levels>();
    }
    return expr;
}


ptr<Expression> Parser::ParseReturnStatement() {
    AdvanceTokens();
    return ParseExpression();
}

ptr<IfStatement> Parser::ParseIfStatement() {
    ptr<IfStatement> if_statement = make_ptr<IfStatement>();
    AdvanceTokens();
    if_statement->condition = ParseExpression();
    if (cur_token_.type != TokenType::then) {
        // missing then
    } else {
        if_statement->to_do = ParseStatement();
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
std::optional<ptr<ElseIfStatement>> Parser::ParseElseIfs() {
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
std::optional<ptr<ElseStatement>> Parser::ParseElseStatement() {
    if (cur_token_.type != TokenType::else_s) {
        return std::nullopt;
    }
    ptr<ElseStatement> ans = make_ptr<ElseStatement>();
    ans->token = cur_token_;
    ans->underlying = ParseStatement();
    return ans;
}

std::vector<ptr<IdentifierExpression>> Parser::ParseArguments() {
    std::vector<ptr<IdentifierExpression>> ans;
    AdvanceTokens(); // skip (
    while(cur_token_.type != TokenType::rparen) {
        ptr<IdentifierExpression> ident_i = make_ptr<IdentifierExpression>();
        ident_i->name = cur_token_.value.value();
        ans.push_back(ident_i);
    }
    AdvanceTokens(); // skip )
    return ans;
}