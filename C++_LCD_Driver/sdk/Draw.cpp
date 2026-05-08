#include "Draw.hpp"
#include "internal/IpcClient.hpp"
#include "../shared/DrawCommand.hpp"

namespace sdk
{

struct Draw::Impl
{
    std::shared_ptr<internal::IpcClient> ipc;
};

Draw::Draw(std::shared_ptr<internal::IpcClient> ipc)
    : impl_(std::make_unique<Impl>())
{
    impl_->ipc = std::move(ipc);
}

Draw::~Draw() = default;

void Draw::clear(uint16_t color)
{
    impl_->ipc->send_command(shared::DrawCommand::Clear, color);
}

void Draw::fill_rect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
{
    impl_->ipc->send_command(shared::DrawCommand::FillRect, x, y, w, h, color);
}

void Draw::draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color)
{
    impl_->ipc->send_command(shared::DrawCommand::DrawRect, x, y, w, h, color);
}

void Draw::fill_circle(int32_t cx, int32_t cy, int32_t r, uint16_t color)
{
    impl_->ipc->send_command(shared::DrawCommand::FillCircle, cx, cy, r, color);
}

void Draw::draw_circle(int32_t cx, int32_t cy, int32_t r, uint16_t color)
{
    impl_->ipc->send_command(shared::DrawCommand::DrawCircle, cx, cy, r, color);
}

void Draw::draw_text(int32_t x, int32_t y, util::font::TextSize size,
                     uint16_t color, std::string_view text)
{
    impl_->ipc->send_command(shared::DrawCommand::DrawText, x, y, size, color, text);
}

void Draw::render()
{
    impl_->ipc->send_command(shared::DrawCommand::Render);
}

}
