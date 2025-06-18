#pragma once

#include <variant>

#include <token.h>
#include <ast/expressions/expression_fwd.h>
#include <ast/statements/statement_fwd.h>

struct Program;

// struct Statement;

struct IfStatement;
struct ElseIfStatement;
struct ElseStatement;
struct AssignStatement;




struct Program {
    std::vector<Statement> statements;
};

struct AssignStatement {
    std::string_view ident;
    Expression expr;
    std::optional<TokenType> funny_assign;
};  

struct ArrayAssignStatement {
    std::string_view array;
    std::vector<Expression> indices;
    Expression value;
    std::optional<TokenType> funny_assign;
};


struct IfStatement {
    Expression condition;
    Statement to_do;
    std::vector<ptr<ElseIfStatement>> else_ifs;
    std::optional<ptr<ElseStatement>> else_s;
};

struct ElseStatement {
    Token token;
    Statement underlying;
};

struct ElseIfStatement {
    Expression condition;
    Statement underlying;
};

struct ReturnStatement {
    Expression value;
};

struct ExprStatement {
    Expression expr;
};

struct WhileStatement {
    Expression condition;
    std::vector<Statement> body;
};

struct BreakStatement {};

struct ContinueStatement {};

struct ForInStatement {
    std::string_view ident;
    Expression range;
    std::vector<Statement> body;
};





