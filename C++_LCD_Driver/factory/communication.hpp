#pragma once
#include "../interface/IFactory.hpp"

class LcdFactoryCommunication : public IFactoryCommunication
{
public:
    std::shared_ptr<IRead>  create_read_interface()     override;
    std::shared_ptr<IWrite> create_write_interface()    override;
};  