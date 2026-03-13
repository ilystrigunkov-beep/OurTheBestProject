#pragma once

#include <stdexcept>
#include <string>

namespace core {

class FileException : public std::runtime_error {
public:
    explicit FileException(const std::string& message);
};

class ParseException : public std::runtime_error {
public:
    explicit ParseException(const std::string& message);
};

} // namespace core

