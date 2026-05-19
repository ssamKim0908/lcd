#pragma once
#include "util/ProcessResult.hpp"

#include <string>

class IProcessLauncher
{
public:
    virtual ~IProcessLauncher() = default;
    virtual ProcessResult launch(const std::string& path) = 0;
};
