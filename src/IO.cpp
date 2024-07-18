#include "IO.h"

Reader::Reader(const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

bool Reader::readLine(std::string& line) {
    if (std::getline(file, line)) {
        return true;
    }
    return false;
}

// Writer implementation
Writer::Writer(const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

void Writer::writeLine(const std::string& line) {
    if (file.is_open()) {
        file << line << std::endl;
    }
}
