#pragma once

class IRenderCommand
{
public:
    virtual ~IRenderCommand() = default;
    virtual void execute() = 0;
};
