#pragma once
#include <string>
#include <vector>

class IFileReader
{
public:
    virtual std::vector<std::string> read_lines(const std::string& path) = 0;
    virtual ~IFileReader() = default;
};
