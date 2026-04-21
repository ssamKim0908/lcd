#pragma once
#include "../interface/ICommunication.hpp"

class LcdReadService : public IRead
{
private:
    std::unique_ptr<IRead> read;
public:
    LcdReadService(std::unique_ptr<IRead> read_interface);
    void read(Span<std::byte> buffer) override;
};

class LcdWriteService : public IWrite
{
private:
    std::unique_ptr<IWrite> write_interface_;
public:
    LcdWriteService(std::unique_ptr<IWrite> write_interface);
    void write(Span<const std::byte> buffer) override;
};
