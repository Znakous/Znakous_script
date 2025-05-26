#pragma once

#include <variant>

#include <token.h>
#include <ast/expressions/expression_fwd.h>

struct Program;

struct Statement;

struct IfStatement;
struct ElseIfStatement;
struct ElseStatement;
struct AssignStatement;


using statement_types = std::variant<ptr<IfStatement>, ptr<AssignStatement>, ptr<ReturnStatement>>;


struct Program {
    std::vector<ptr<Statement>> statements;
};

struct Statement {
    statement_types value;
};
struct AssignStatement {
    ptr<IdentifierExpression> ident;
    ptr<Expression> expr;
};

struct IfStatement {
    ptr<Expression> condition;
    ptr<Statement> to_do;
    std::vector<ptr<ElseIfStatement>> else_ifs;
    std::optional<ptr<ElseStatement>> else_s;
};

struct ElseStatement {
    Token token;
    ptr<Statement> underlying;
};

struct ElseIfStatement {
    ptr<Expression> condition;
    ptr<Statement> underlying;
};

struct ReturnStatement {
    ptr<Expression> value;
};