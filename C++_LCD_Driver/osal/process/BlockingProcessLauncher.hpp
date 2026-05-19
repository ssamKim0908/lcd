#pragma once
#include "interface/IProcessLauncher.hpp"

class BlockingProcessLauncher : public IProcessLauncher
{
public:
    ProcessResult launch(const std::string& path) override;
};
