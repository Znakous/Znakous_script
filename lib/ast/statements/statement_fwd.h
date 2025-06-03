#pragma once

struct Program;

// struct Statement;

struct IfStatement;
struct ElseIfStatement;
struct ElseStatement;
struct AssignStatement;
struct ExprStatement; // todo

struct ReturnStatement;

using Statement = std::variant<ptr<IfStatement>, ptr<AssignStatement>, ptr<ReturnStatement>>;
