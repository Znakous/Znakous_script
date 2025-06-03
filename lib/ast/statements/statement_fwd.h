#pragma once

#include <usings.h>

struct Program;

// struct Statement;

struct IfStatement;
struct ElseIfStatement;
struct ElseStatement;
struct AssignStatement;
struct WhileStatement; // todo
struct ExprStatement; // todo

struct ReturnStatement;

using Statement = std::variant<ptr<WhileStatement>, ptr<ExprStatement>, ptr<IfStatement>, ptr<AssignStatement>, ptr<ReturnStatement>>;
