#pragma once

#include <string>
#include <variant>
#include <stdexcept>
#include <memory>
#include <cmath>

#include <usings.h>
#include <object.h>
#include <logger.h>

constexpr double True_val = 1.0;
constexpr double False_val = 0.0;


struct BinaryOperVisitor {
    BinaryOperVisitor()
     : logger_(make_ptr<logging::Logger>())
    {}
    BinaryOperVisitor(std::shared_ptr<logging::Logger> log)
     : logger_(log) 
    {}
    virtual ~BinaryOperVisitor() = default;
    virtual Object operator()(Object&, Object&) = 0;
protected:
    std::shared_ptr<logging::Logger> logger_;
};


struct UnaryOperVisitor {
    UnaryOperVisitor()
     : logger_(make_ptr<logging::Logger>())
    {}
    UnaryOperVisitor(std::shared_ptr<logging::Logger> log)
     : logger_(log) 
    {}
    virtual ~UnaryOperVisitor() = default;
    virtual Object operator()(Object&) = 0;
protected:
    std::shared_ptr<logging::Logger> logger_;
};