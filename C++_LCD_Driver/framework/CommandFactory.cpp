#include "CommandFactory.hpp"
#include "commands/RenderCommands.hpp"
#include "../shared/Packet.hpp"
#include "../shared/DrawCommand.hpp"
#include "../util/Font.hpp"

#include <cstring>
#include <stdexcept>
#include <utility>

namespace
{
    template <typename T>
    T read_le(const std::byte* p)
    {
        T v{};
        std::memcpy(&v, p, sizeof(T));
        return v;
    }
}

SimpleCommandFactory::SimpleCommandFactory(std::shared_ptr<Rasterizer> receiver)
    : receiver(std::move(receiver)) {}

SimpleCommandFactory::~SimpleCommandFactory() = default;

std::unique_ptr<ICommand> SimpleCommandFactory::create(const Packet& packet)
{
    if (packet.data.empty())
        throw std::runtime_error("SimpleCommandFactory: empty packet");

    const auto       op   = static_cast<shared::DrawCommand>(packet.data[0]);
    const std::byte* body = packet.data.data() + sizeof(shared::DrawCommand);

    switch (op)
    {
    case shared::DrawCommand::Clear:
    {
        auto color = read_le<uint16_t>(body);
        return std::make_unique<ClearCommand>(receiver, color);
    }
    case shared::DrawCommand::FillRect:
    {
        auto x = read_le<int32_t> (body +  0);
        auto y = read_le<int32_t> (body +  4);
        auto w = read_le<int32_t> (body +  8);
        auto h = read_le<int32_t> (body + 12);
        auto c = read_le<uint16_t>(body + 16);
        return std::make_unique<FillRectCommand>(receiver, x, y, w, h, c);
    }
    case shared::DrawCommand::DrawRect:
    {
        auto x = read_le<int32_t> (body +  0);
        auto y = read_le<int32_t> (body +  4);
        auto w = read_le<int32_t> (body +  8);
        auto h = read_le<int32_t> (body + 12);
        auto c = read_le<uint16_t>(body + 16);
        return std::make_unique<DrawRectCommand>(receiver, x, y, w, h, c);
    }
    case shared::DrawCommand::FillCircle:
    {
        auto cx = read_le<int32_t> (body +  0);
        auto cy = read_le<int32_t> (body +  4);
        auto r  = read_le<int32_t> (body +  8);
        auto c  = read_le<uint16_t>(body + 12);
        return std::make_unique<FillCircleCommand>(receiver, cx, cy, r, c);
    }
    case shared::DrawCommand::DrawCircle:
    {
        auto cx = read_le<int32_t> (body +  0);
        auto cy = read_le<int32_t> (body +  4);
        auto r  = read_le<int32_t> (body +  8);
        auto c  = read_le<uint16_t>(body + 12);
        return std::make_unique<DrawCircleCommand>(receiver, cx, cy, r, c);
    }
    case shared::DrawCommand::DrawText:
    {
        auto x        = read_le<int32_t> (body + 0);
        auto y        = read_le<int32_t> (body + 4);
        auto text_len = read_le<uint16_t>(body + 8);
        
        const char* chars = reinterpret_cast<const char*>(body + 10);
        std::string text(chars, text_len);

        auto size     = static_cast<util::font::TextSize>(
                            read_le<uint8_t>(body + 10 + text_len));
        auto color    = read_le<uint16_t>(body + 11 + text_len);

        return std::make_unique<DrawTextCommand>(
            receiver, x, y, std::move(text), size, color);
    }
    case shared::DrawCommand::Render:
    {
        return std::make_unique<RenderCommand>(receiver);
    }
    }
    throw std::runtime_error("SimpleCommandFactory: unknown DrawCommand");
}
