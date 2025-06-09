#include "interpreter.h"

bool interpret(std::istream& input, std::ostream& output) {
    Interpreter inter(input, output);
    auto prog = inter.ParseProgram();
    inter.Evaluate(prog);   
    return true;
}