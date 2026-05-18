#include "IpcSFromC.hpp"
#include "commands/RenderCommands.hpp"
#include "../shared/Packet.hpp"
#include "../shared/MsgType.hpp"
#include "../util/IpcSerializer.hpp"

#include <stdexcept>
#include <utility>

SimpleCommandFactory::SimpleCommandFactory(std::shared_ptr<Rasterizer> receiver)
    : receiver(std::move(receiver)) {}

SimpleCommandFactory::~SimpleCommandFactory() = default;

std::unique_ptr<ICommand> SimpleCommandFactory::create(const Packet& packet)
{
    if (packet.data.empty())
        throw std::runtime_error("SimpleCommandFactory: empty packet");

    util::ipc::Reader r(packet.data.data(), packet.data.size());
    const auto type = static_cast<shared::C2S>(r.get_u8());

    switch (type)
    {
    case shared::C2S::Clear:
    {
        auto color = r.get_u16();
        return std::make_unique<ClearCommand>(receiver, color);
    }
    case shared::C2S::FillRect:
    {
        auto x = r.get_i32();
        auto y = r.get_i32();
        auto w = r.get_i32();
        auto h = r.get_i32();
        auto c = r.get_u16();
        return std::make_unique<FillRectCommand>(receiver, x, y, w, h, c);
    }
    case shared::C2S::DrawRect:
    {
        auto x = r.get_i32();
        auto y = r.get_i32();
        auto w = r.get_i32();
        auto h = r.get_i32();
        auto c = r.get_u16();
        return std::make_unique<DrawRectCommand>(receiver, x, y, w, h, c);
    }
    case shared::C2S::FillCircle:
    {
        auto cx = r.get_i32();
        auto cy = r.get_i32();
        auto rr = r.get_i32();
        auto c  = r.get_u16();
        return std::make_unique<FillCircleCommand>(receiver, cx, cy, rr, c);
    }
    case shared::C2S::DrawCircle:
    {
        auto cx = r.get_i32();
        auto cy = r.get_i32();
        auto rr = r.get_i32();
        auto c  = r.get_u16();
        return std::make_unique<DrawCircleCommand>(receiver, cx, cy, rr, c);
    }
    case shared::C2S::DrawText:
    {
        auto x     = r.get_i32();
        auto y     = r.get_i32();
        auto size  = r.get_text_size();
        auto text  = r.get_string();
        auto color = r.get_u16();
        return std::make_unique<DrawTextCommand>(
            receiver, x, y, std::move(text), size, color);
    }
    case shared::C2S::Render:
        return std::make_unique<RenderCommand>(receiver);
    }
    throw std::runtime_error("SimpleCommandFactory: unknown C2S type");
}
