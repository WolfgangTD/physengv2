#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::string> loadNoteFiles(const std::string& filename) {
    std::vector<std::string> noteFiles;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open " << filename << "\n";
        return noteFiles;
    }

    std::string line;
    while (std::getline(file, line)) {
        noteFiles.push_back(line);
    }

    file.close();
    return noteFiles;
}