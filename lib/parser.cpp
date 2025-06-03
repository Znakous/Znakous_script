#include "parser.h"

Parser::Parser(const char* data) 
    : lexer_(data)
{    
    cur_token_ = lexer_.GetToken();
    peek_token_ = lexer_.GetToken();
    std::cout << "init cur and peek\n";
    std::cout << (int)cur_token_.type << " " << (int)peek_token_.type << "\n";
    // initialize cur and peek tokens
}
void Parser::AdvanceTokens() {
    std::cout << "before advance\n";
    std::cout << (int)cur_token_.type << " " << (int)peek_token_.type << "\n";
    cur_token_ = peek_token_;
    peek_token_ = lexer_.GetToken();
    std::cout << "after advance\n";
    std::cout << (int)cur_token_.type << " " << (int)peek_token_.type << "\n";
}
Statement Parser::ParseStatement() {
    std::cout << "parsing statement\n";
    std::cout << (int) cur_token_.type << " type\n";
    std::cout << (int) peek_token_.type << " peek type\n";
    if (cur_token_.type != TokenType::invalid) {
        if (cur_token_.type == TokenType::if_s) {
            std::cout << "---------- its if\n";
            return ParseIfStatement();
        } else if (cur_token_.type == TokenType::ident 
                    && peek_token_.type == TokenType::assign) {
            std::cout << "to assign\n";
            return ParseAssignStatement();
        } else if (cur_token_.type == TokenType::ret) {
            return ParseReturnStatement();
        }
    } else {
        std::cout << "invalid statement\n";
        // pupupu, wrong statement
    }
    std::cout << "wrong parsed statement\n";
}

ptr<AssignStatement> Parser::ParseAssignStatement() {
    ptr<AssignStatement> ans = make_ptr<AssignStatement>();
    ans->ident = cur_token_.value.value();
    std::cout << "in asssign\n";
    std::cout << (int) cur_token_ << " <-";
    AdvanceTokens();
    std::cout << (int) cur_token_ << " <-";
    AdvanceTokens();
    std::cout << (int) cur_token_ << " <-";
    ans->expr = ParseExpression();
    return ans;
}

ptr<Program> Parser::ParseProgram() {
    std::cout << "parsing program\n";
    ptr<Program> ans = make_ptr<Program>();
    while (cur_token_.type != TokenType::invalid) {
        std::cout << "parse stmt\n";
        ans->statements.emplace_back(ParseStatement());
        std::cout << "parsed stmt\n";
        std::cout << "after parse " << (int) cur_token_.type << "\n";
        
    }
    return ans;
}

Expression Parser::ParseExpression() {
    std::cout << "parsing expression\n";
    // Expression expr = make_ptr<Expression>();
    if (cur_token_.type == TokenType::func) {
        std::cout << "i met func\n";
        AdvanceTokens(); // skip func
        // AdvanceTokens(); // skip lparen
        ptr<FunctionalExpression> ans = make_ptr<FunctionalExpression>();
        std::cout << "parse args\n";
        while (cur_token_.type != TokenType::rparen) {
            std::cout << "arg next\n";
            AdvanceTokens();
            std::cout << "arg\n";
            ans->arguments.push_back(cur_token_.value.value());
            AdvanceTokens();
        }
        std::cout << "go advance ret\n";
        AdvanceTokens();
        std::cout << "go parse ret\n";
        while (cur_token_.type != TokenType::endfunc) {
            ans->body.push_back(ParseStatement());
        }
        // ans = ParseReturnStatement();
        std::cout << "i parsed func\n";
        AdvanceTokens();
        return ans;
    } else {
        return ParseLeveledExpression<operators_levels>();
    }
    // return expr;
}


ptr<ReturnStatement> Parser::ParseReturnStatement() {
    ptr<ReturnStatement> ans = make_ptr<ReturnStatement>();
    AdvanceTokens();
    ans->value = ParseExpression();
    std::cout << "parse ret\n";
    return ans;
}

ptr<IfStatement> Parser::ParseIfStatement() {
    ptr<IfStatement> if_statement = make_ptr<IfStatement>();
    std::cout << "boom if......\n";
    std::cout << (int) cur_token_.type << " pppppp\n";
    AdvanceTokens();
    std::cout << (int) cur_token_.type << " pppppp\n";
    std::cout << "boom if2......\n";
    if_statement->condition = ParseExpression();
    std::cout << "                parsed cond\n";
    if (cur_token_.type != TokenType::then) {
        // missing then
    } else {
        AdvanceTokens();
        if_statement->to_do = ParseStatement();
        std::cout << (int) cur_token_.type << " ttttt\n";
        while(true) {
            auto else_if = ParseElseIfs();
            if (else_if.has_value()) {
                if_statement->else_ifs.push_back(else_if.value());
            } else {
                break;
            }
        }
        std::cout << "go parse else <---------------\n";
        std::cout << (int) cur_token_.type << " kkkkkk\n";
        if_statement->else_s = ParseElseStatement();
    }
    AdvanceTokens();
    return if_statement;
}
std::optional<ptr<ElseIfStatement>> Parser::ParseElseIfs() {
    if (cur_token_.type == TokenType::elif_s) {
        std::cout << "aaaaaaaaaaaaaaaaaaaaaa\n";
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
    AdvanceTokens();
    ans->underlying = ParseStatement();
    return ans;
}

std::vector<Expression> Parser::ParseArguments() {
    std::cout << "parse args 2\n";
    std::vector<Expression> ans;
    // AdvanceTokens(); // skip (
    while(cur_token_.type != TokenType::rparen) {
        std::cout << "arg 2\n";
        AdvanceTokens();
        ans.push_back(ParseExpression());
        // AdvanceTokens(); // check 
    }
    std::cout << "skip )\n";
    AdvanceTokens(); // skip )
    return ans;
}