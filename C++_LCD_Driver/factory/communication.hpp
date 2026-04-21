#pragma once
#include "../interface/IFactory.hpp"

class Gpio;

class LcdFactoryCommunication : public IFactoryCommunication
{
private:
    std::unique_ptr<Gpio> gpio;
public:
    LcdFactoryCommunication();
    ~LcdFactoryCommunication();
private:
    int get_gpio_fd();
public:
    std::unique_ptr<IRead>  create_read_interface()     override;
    std::unique_ptr<IWrite> create_write_interface()    override;

};  