#include "controller.hpp"

Controller::Controller(std::unique_ptr<IFactoryCommunication> factory) : Factory(std::move(factory))
{
    ReadInterface  = Factory->create_read_interface();
    WriteInterface = Factory->create_write_interface();
}

void Controller::read(Span<std::byte> buffer)
{
    std::cout << "Reading from controller" << std::endl;
#ifdef TARGET_DEVICE
    ReadInterface->read(buffer);
#endif
}

void Controller::write(Span<const std::byte> buffer)
{
    std::cout << "Writing to controller" << std::endl;
#ifdef TARGET_DEVICE
    WriteInterface->write(buffer);
#endif
}