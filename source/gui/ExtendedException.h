#pragma once

#include <exception>
#include <string>

class ExtendedException : public std::exception {
public:
    ExtendedException(const std::string& message, const char* file, int line);

    std::string ewhat() const noexcept;
    const char* getFile() const noexcept;
    int getLine() const noexcept;

private:
    std::string errorMessage;
    std::string errorFile;
    int errorLine;
};

#define THROW_EXTENDED_EXCEPTION(msg) throw ExtendedException(msg, __FILE__, __LINE__)