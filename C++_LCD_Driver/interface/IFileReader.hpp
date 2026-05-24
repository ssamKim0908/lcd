#pragma once
#include <string>
#include <vector>

class IFileReader
{
public:
    virtual std::vector<std::string> read_lines(const std::string& path) = 0;

    virtual std::vector<std::string> list_files(const std::string& dir,
                                                const std::string& ext) = 0;

    virtual ~IFileReader() = default;
};
