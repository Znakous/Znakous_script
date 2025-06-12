#pragma once

#include <string>
#include <istream>

std::string PreprocessWild(const std::string& code);
std::string PreprocessWild(std::istream& in);
std::string PreprocessNormal(const std::string& code);
std::string PreprocessNormal(std::istream& in);