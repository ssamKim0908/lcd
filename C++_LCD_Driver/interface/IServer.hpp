#pragma once
#include <memory>

class IChannel;

class IServer
{
public:
    virtual std::unique_ptr<IChannel> accept()    = 0;
    virtual int                       fd() const  = 0;
    virtual ~IServer() = default;
};
