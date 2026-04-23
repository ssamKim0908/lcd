#pragma once
#include "../include/common.h"
#include "../interface/ICommunication.hpp"
#include "../interface/IFactory.hpp"

class Controller
{
private:
    std::unique_ptr<IFactoryCommunication>  Factory;
    std::unique_ptr<IRead>                  ReadInterface;
    std::unique_ptr<IWrite>                 WriteInterface;
public:
    Controller(std::unique_ptr<IFactoryCommunication> factory);
    
    Controller(const Controller&)            = delete;
    Controller& operator=(const Controller&) = delete;

    Controller(const Controller&&)            = delete;
    Controller& operator=(const Controller&&) = delete;

    void read   (Span<std::byte> buffer);
    void write  (Span<const std::byte> buffer);

    ~Controller() = default;
};

