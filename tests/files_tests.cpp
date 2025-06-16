#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include "lib/interpreter.h"

TEST(FilesTests, InterpretFiles) {
    const std::filesystem::path test_files_dir = "tests/files";
    const std::filesystem::path expected_files_dir = "tests/expected";
    
    for (const auto& entry : std::filesystem::directory_iterator(test_files_dir)) {
        if (entry.path().extension() == ".is") {
            std::ostringstream output;
            
            ASSERT_TRUE(interpret_file(entry.path().string(), output)) 
                << "Failed to interpret file: " << entry.path();
            
            auto expected_file = expected_files_dir / entry.path().filename();
            expected_file.replace_extension(".txt");
            
            std::ifstream expected_stream(expected_file);
            ASSERT_TRUE(expected_stream.is_open()) 
                << "Failed to open expected output file: " << expected_file;
            
            std::string expected_output((std::istreambuf_iterator<char>(expected_stream)),
                                      std::istreambuf_iterator<char>());
            std::string actual_output = output.str();
            
            // expected_output.erase(std::remove(expected_output.begin(), expected_output.end(), '\r'), expected_output.end());
            // actual_output.erase(std::remove(actual_output.begin(), actual_output.end(), '\r'), actual_output.end());
            
            ASSERT_EQ(actual_output, expected_output)
                << "Output does not match for file: " << entry.path() << "\n"
                << "Expected:\n" << expected_output << "\n"
                << "Actual:\n" << actual_output;
        }
    }
}

