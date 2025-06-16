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

bool interpret_file(std::string_view file_name, std::ostream& output) {
    std::ifstream file(file_name.data());
    if (!file.is_open()) {
        output << "Failed to open file: " << file_name << std::endl;
        return false;
    }
    return interpret(file, output);
}