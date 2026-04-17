#include "controller.hpp"

Controller::Controller(std::unique_ptr<IFactoryCommunication> factory) : Factory(std::move(factory))
{
    ReadInterface  = Factory->create_read_interface();
    WriteInterface = Factory->create_write_interface();
}