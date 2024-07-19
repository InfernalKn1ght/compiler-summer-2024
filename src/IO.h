#pragma once

#include <string>
#include <fstream>
#include <iostream>

class Reader {
public:
    Reader(const std::string& filename);
    bool readLine(std::string& line);

private:
    std::ifstream file;
};

class Writer {
public:
    Writer(const std::string& filename);
    void writeLine(const std::string& line);

private:
    std::ofstream file;
};
