#pragma once
#include "../../interface/IFileReader.hpp"

class LocalFileReader : public IFileReader
{
public:
    std::vector<std::string> read_lines(const std::string& path) override;
};
