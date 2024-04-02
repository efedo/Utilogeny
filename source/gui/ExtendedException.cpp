#include "ExtendedException.h"
#include <sstream>

ExtendedException::ExtendedException(const std::string& message, const char* file, int line)
    : errorMessage(message), errorFile(file), errorLine(line) {}

std::string ExtendedException::ewhat() const noexcept {
    std::ostringstream oss;
    oss << errorMessage << " in file " << errorFile << ", line " << errorLine;
    return oss.str();
}

const char* ExtendedException::getFile() const noexcept {
    return errorFile.c_str();
}

int ExtendedException::getLine() const noexcept {
    return errorLine;
}