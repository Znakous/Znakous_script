#pragma once

#include <iostream>


#include "preprocess.h"
#include "lexer.h"
#include "parser.h"



struct Interpreter{
    Interpreter(const std::string& data) 
     : data_(Preprocessor(data).GetCode()), parser_(data_) 
    {}
    
private:
    Parser parser_;
    const char* data_;
};
















bool interpret(std::istream& input, std::ostream& output);