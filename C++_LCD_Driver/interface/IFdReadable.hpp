#pragma once

class IFdReadable
{
public:
    virtual int fd() const = 0;
    virtual ~IFdReadable() = default;
};
