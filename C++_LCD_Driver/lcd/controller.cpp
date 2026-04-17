#include "controller.hpp"

Controller::Controller(std::unique_ptr<IFactoryCommunication> factory) : Factory(std::move(factory))
{
    ReadInterface  = Factory->create_read_interface();
    WriteInterface = Factory->create_write_interface();
}

void Controller::read(span<std::byte> buffer)
{
    std::cout << "Reading from controller" << std::endl;
    //ReadInterface->read(buffer);
}

void Controller::write(span<const std::byte> buffer)
{
    std::cout << "Writing to controller" << std::endl;

    //WriteInterface->write(buffer);
}