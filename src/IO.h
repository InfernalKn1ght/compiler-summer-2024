#pragma once

#include <string>
#include <fstream>
#include <iostream>

class Reader {
public:
    Reader(const std::string& filename);
	~Reader();
	std::string readFile();

private:
    std::ifstream file;
};

class Writer {
public:
    Writer(const std::string& filename);
	~Writer();
    void writeLine(const std::string& line);

private:
    std::ofstream file;
};

