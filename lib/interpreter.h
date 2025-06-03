#pragma once

#include <iostream>
#include <functional>


#include "preprocess.h"
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"



struct Interpreter{
    Interpreter(const std::string& data, std::ostream& output) 
     : parser_(data.data()), output_(output), eval_(output)
    {std::cout << "constructed\n";}
    ptr<Program> ParseProgram() {
        std::cout << "interpreter parses program\n";
        return parser_.ParseProgram();
    }
    void Evaluate(ptr<Program> prog) {
        for (auto& st : prog->statements) {
            std::cout << "evaling statement\n";
            std::visit(eval_, st);
        }
    }
    
private:
    std::reference_wrapper<std::ostream> output_;
    Evaluator eval_;
    Parser parser_;
    std::string data_;
};



bool interpret(std::istream& input, std::ostream& output);














bool interpret(std::istream& input, std::ostream& output);