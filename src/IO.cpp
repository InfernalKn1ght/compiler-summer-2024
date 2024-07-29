#include "IO.h"

Reader::Reader(const std::string& filename) {
    file.open(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

std::string Reader::readFile() {
	std::string content( (std::istreambuf_iterator<char>(file) ),
						   (std::istreambuf_iterator<char>()    ) );
	return content;

}

Reader::~Reader(){
	if (file.is_open()){
		file.close();
	}
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
        file << line;
    }
}

Writer::~Writer(){
	if (file.is_open()){
		file.close();
	}
}
