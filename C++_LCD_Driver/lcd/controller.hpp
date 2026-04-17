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
    
    void read   (span<std::byte> buffer);
    void write  (span<const std::byte> buffer);

    ~Controller() = default;
};

