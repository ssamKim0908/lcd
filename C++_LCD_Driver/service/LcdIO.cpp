#include "LcdIO.hpp"

LcdReadService::LcdReadService(std::unique_ptr<IRead> read_interface) 
    : read_interface_(std::move(read_interface)) {}

void LcdReadService::read(Span<std::byte> buffer)
{
    if (read_interface_) {
        read_interface_->read(buffer);
    }
}

LcdWriteService::LcdWriteService(std::unique_ptr<IWrite> write_interface) 
    : write_interface_(std::move(write_interface)) {}

void LcdWriteService::write(Span<const std::byte> buffer)
{
    if (write_interface_) {
        write_interface_->write(buffer);
    }
}
