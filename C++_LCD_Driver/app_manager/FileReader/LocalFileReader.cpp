#include "LocalFileReader.hpp"

#include <fstream>
#include <stdexcept>

std::vector<std::string> LocalFileReader::read_lines(const std::string& path)
{
    std::ifstream in(path);
    if (!in) throw std::runtime_error("LocalFileReader: cannot open " + path);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) lines.push_back(std::move(line));
    return lines;
}
