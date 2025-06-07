#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <sstream>

namespace logging {

class Logger {
public:
    Logger() = default;
    ~Logger() {
        #ifdef DEBUG_BUILD
        if (logFile.is_open()) {
            logFile.close();
        }
        #endif
    }

    static std::shared_ptr<Logger> create() {
        return std::make_shared<Logger>();
    }

    void setLogFile(const std::string& filename) {
        #ifdef DEBUG_BUILD
        if (logFile.is_open()) {
            logFile.close();
        }
        logFile.open(filename, std::ios::app);
        #endif
    }

    template<typename... Args>
    void log(Args&&... args) {
        #ifdef DEBUG_BUILD
        std::ostringstream message;
        (message << ... << args);
        
        if (logFile.is_open()) {
            logFile << message.str() << std::endl;
        }
        std::cout << message.str() << std::endl;
        #endif
    }

private:
    #ifdef DEBUG_BUILD
    std::ofstream logFile;
    #endif
};

} // namespace logging

