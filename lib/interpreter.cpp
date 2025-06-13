#include "interpreter.h"

bool interpret(std::istream& input, std::ostream& output) {
    try {
        Interpreter inter(input, output);
        auto prog = inter.ParseProgram();
        inter.Evaluate(prog);   
        return true;
    } catch (const std::exception& e) {
        output << e.what() << std::endl;
        return false;
    }
}