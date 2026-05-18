#include "IpcCToS.hpp"
#include "../../interface/IChannel.hpp"
#include "../../shared/MsgType.hpp"
#include "../../util/span.hpp"

#include <utility>

namespace sdk::internal
{

IpcCToS::IpcCToS(std::shared_ptr<ISender> sender)
    : sender_(std::move(sender)) {}

IpcCToS::~IpcCToS() = default;


void IpcCToS::clear(uint16_t color)
{
    buffer_.clear();
    writer_.put_u8 (buffer_, static_cast<uint8_t>(shared::C2S::Clear));
    writer_.put_u16(buffer_, color);
    sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}

void IpcCToS::fill_rect(int x, int y, int w, int h, uint16_t color)
{
    buffer_.clear();
    writer_.put_u8 (buffer_, static_cast<uint8_t>(shared::C2S::FillRect));
    writer_.put_i32(buffer_, x);
    writer_.put_i32(buffer_, y);
    writer_.put_i32(buffer_, w);
    writer_.put_i32(buffer_, h);
    writer_.put_u16(buffer_, color);
    sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}

void IpcCToS::draw_rect(int x, int y, int w, int h, uint16_t color)
{
    buffer_.clear();
    writer_.put_u8 (buffer_, static_cast<uint8_t>(shared::C2S::DrawRect));
    writer_.put_i32(buffer_, x);
    writer_.put_i32(buffer_, y);
    writer_.put_i32(buffer_, w);
    writer_.put_i32(buffer_, h);
    writer_.put_u16(buffer_, color);
    sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}

void IpcCToS::fill_circle(int cx, int cy, int r, uint16_t color)
{
    buffer_.clear();
    writer_.put_u8 (buffer_, static_cast<uint8_t>(shared::C2S::FillCircle));
    writer_.put_i32(buffer_, cx);
    writer_.put_i32(buffer_, cy);
    writer_.put_i32(buffer_, r);
    writer_.put_u16(buffer_, color);
    sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}

void IpcCToS::draw_circle(int cx, int cy, int r, uint16_t color)
{
    buffer_.clear();
    writer_.put_u8 (buffer_, static_cast<uint8_t>(shared::C2S::DrawCircle));
    writer_.put_i32(buffer_, cx);
    writer_.put_i32(buffer_, cy);
    writer_.put_i32(buffer_, r);
    writer_.put_u16(buffer_, color);
    sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}

void IpcCToS::draw_text(int x, int y, const std::string& text,
                        util::font::TextSize size, uint16_t color)
{
    buffer_.clear();
    writer_.put_u8       (buffer_, static_cast<uint8_t>(shared::C2S::DrawText));
    writer_.put_i32      (buffer_, x);
    writer_.put_i32      (buffer_, y);
    writer_.put_string   (buffer_, text);
    writer_.put_text_size(buffer_, size);
    writer_.put_u16      (buffer_, color);
    sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}

void IpcCToS::render()
{
    buffer_.clear();
    writer_.put_u8(buffer_, static_cast<uint8_t>(shared::C2S::Render));
    sender_->send(util::Span<const std::byte>{ buffer_.data(), buffer_.size() });
}

}
