#pragma once

#include <usings.h>

struct Program;

// struct Statement;

struct IfStatement;
struct ElseIfStatement;
struct ElseStatement;
struct AssignStatement;
struct WhileStatement;
struct ExprStatement;
struct ArrayAssignStatement;

struct ReturnStatement;
struct BreakStatement;
struct ContinueStatement;
struct ForInStatement;

using Statement = std::variant<ptr<WhileStatement>, 
                                ptr<ExprStatement>, 
                                ptr<IfStatement>, 
                                ptr<AssignStatement>, 
                                ptr<ReturnStatement>, 
                                ptr<ArrayAssignStatement>,
                                ptr<BreakStatement>, 
                                ptr<ContinueStatement>, 
                                ptr<ForInStatement>
>;
