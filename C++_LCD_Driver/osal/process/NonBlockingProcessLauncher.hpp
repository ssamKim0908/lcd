#pragma once
#include "interface/IProcessLauncher.hpp"

class NonBlockingProcessLauncher : public IProcessLauncher
{
public:
    ProcessResult launch(const std::string& path) override;
};
