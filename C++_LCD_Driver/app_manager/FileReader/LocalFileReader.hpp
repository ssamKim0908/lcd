#pragma once
#include "../../interface/IFileReader.hpp"

class LocalFileReader : public IFileReader
{
public:
    std::vector<std::string> read_lines(const std::string& path) override;
    std::vector<std::string> list_files(const std::string& dir,
                                        const std::string& ext) override;
};
