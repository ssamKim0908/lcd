#pragma once
#include "../interface/IFactory.hpp"

class LcdFactoryCommunication : public IFactoryCommunication
{
public:
    std::unique_ptr<IRead>  create_read_interface()     override;
    std::unique_ptr<IWrite> create_write_interface()    override;
};  