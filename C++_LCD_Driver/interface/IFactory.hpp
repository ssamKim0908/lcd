#pragma once
#include "ICommunication.hpp"

class IFactoryCommunication
{
public:
    virtual std::unique_ptr<IRead>  create_read_interface() = 0;
    virtual std::unique_ptr<IWrite> create_write_interface() = 0;
    virtual ~IFactoryCommunication() = default;
};

class FactoryCommunication : public IFactoryCommunication
{
public:
    std::unique_ptr<IRead>  create_read_interface()     override;
    std::unique_ptr<IWrite> create_write_interface()    override;
};  