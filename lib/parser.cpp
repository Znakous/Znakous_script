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
            return ParseIfStatement();
        } else if (cur_token_.type == TokenType::ident 
                    && peek_token_.type == TokenType::assign) {
            return ParseAssignStatement();
        }
    } else {
        // pupupu, wrong statement
    }
}

ptr<AssignStatement> Parser::ParseAssignStatement() {
    ptr<AssignStatement> ans = make_ptr<AssignStatement>();
    ans->ident = make_ptr<Identifier>();
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
    // do smth
    return expr;
}


ptr<IfStatement> Parser::ParseIfStatement() {
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