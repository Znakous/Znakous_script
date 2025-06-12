#pragma once

#include <iostream>
#include <functional>


#include "preprocess.h"
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"
#include "logger.h"



struct Interpreter{
    Interpreter(const std::string& data, std::ostream& output, bool wild_lexer=false) 
     : output_(output),
       logger_(logging::Logger::create()),
       eval_(output, logger_),
       parser_(data.data(), logger_, wild_lexer)
    {
        #ifdef DEBUG_BUILD
        logger_->setLogFile(log_file);
        logger_->log("Debug mode enabled - logging to console and file: ", log_file);
        logger_->log("Interpreter constructed");
        #endif
    }
    Interpreter(std::istream& input, std::ostream& output, bool wild_lexer=false) 
     : output_(output),
       logger_(logging::Logger::create()),
       eval_(output, logger_),
       parser_(input, logger_, wild_lexer)
    {
        #ifdef DEBUG_BUILD
        logger_->setLogFile(log_file);
        logger_->log("Debug mode enabled - logging to console and file: ", log_file);
        logger_->log("Interpreter constructed");
        #endif
    }
    ptr<Program> ParseProgram() {
        logger_->log("Starting program parsing");
        return parser_.ParseProgram();
    }
    void Evaluate(ptr<Program> prog) {
        for (auto& st : prog->statements) {
            logger_->log("Evaluating statement");
            std::visit(eval_, st);
        }
    }
    
private:
    std::reference_wrapper<std::ostream> output_;
    std::shared_ptr<logging::Logger> logger_;
    Evaluator eval_;
    Parser parser_;
    std::string data_;
    std::string log_file = "interpreter_log.txt";
};



bool interpret(std::istream& input, std::ostream& output);

