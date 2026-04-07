#pragma once
#include "../include/pch.h"
#include "../interface/ICommunication.hpp"


class IFactoryCommunication
{
public:
    virtual std::shared_ptr<IRead>  create_read_interface() = 0;
    virtual std::shared_ptr<IWrite> create_write_interface() = 0;
    virtual ~IFactoryCommunication() = default;
};

class FactoryCommunication : public IFactoryCommunication
{
public:
    std::shared_ptr<IRead>  create_read_interface()     override;
    std::shared_ptr<IWrite> create_write_interface()    override;
};  