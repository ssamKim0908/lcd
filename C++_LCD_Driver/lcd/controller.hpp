#pragma once
#include "../include/pch.h"
#include "../interface/ICommunication.hpp"
#include "../interface/IFactory.hpp"

class Controller
{
private:
    std::shared_ptr<IFactoryCommunication> Factory;
    std::shared_ptr<IRead>  ReadInterface;
    std::shared_ptr<IWrite> WriteInterface;
public:
    Controller(std::shared_ptr<IFactoryCommunication> factory) : Factory{factory}
    {
        ReadInterface = Factory->create_read_interface();
        WriteInterface = Factory->create_write_interface();
    }

    ~Controller() = default;
};

