#include "IpcCToS.hpp"
#include "../../../interface/IChannel.hpp"
#include "../../../shared/MsgType.hpp"

#include <utility>

namespace sdk::internal
{

IpcCToS::IpcCToS(std::shared_ptr<ISender> sender)
    : sender_(std::move(sender)) {}

IpcCToS::~IpcCToS() = default;


void IpcCToS::clear(uint16_t color)
{
    writer_.clear();
    writer_.put_u8 (static_cast<uint8_t>(shared::C2S::Clear));
    writer_.put_u16(color);
    sender_->send(writer_.bytes());
}

void IpcCToS::fill_rect(int x, int y, int w, int h, uint16_t color)
{
    writer_.clear();
    writer_.put_u8 (static_cast<uint8_t>(shared::C2S::FillRect));
    writer_.put_i32(x);
    writer_.put_i32(y);
    writer_.put_i32(w);
    writer_.put_i32(h);
    writer_.put_u16(color);
    sender_->send(writer_.bytes());
}

void IpcCToS::draw_rect(int x, int y, int w, int h, uint16_t color)
{
    writer_.clear();
    writer_.put_u8 (static_cast<uint8_t>(shared::C2S::DrawRect));
    writer_.put_i32(x);
    writer_.put_i32(y);
    writer_.put_i32(w);
    writer_.put_i32(h);
    writer_.put_u16(color);
    sender_->send(writer_.bytes());
}

void IpcCToS::fill_circle(int cx, int cy, int r, uint16_t color)
{
    writer_.clear();
    writer_.put_u8 (static_cast<uint8_t>(shared::C2S::FillCircle));
    writer_.put_i32(cx);
    writer_.put_i32(cy);
    writer_.put_i32(r);
    writer_.put_u16(color);
    sender_->send(writer_.bytes());
}

void IpcCToS::draw_circle(int cx, int cy, int r, uint16_t color)
{
    writer_.clear();
    writer_.put_u8 (static_cast<uint8_t>(shared::C2S::DrawCircle));
    writer_.put_i32(cx);
    writer_.put_i32(cy);
    writer_.put_i32(r);
    writer_.put_u16(color);
    sender_->send(writer_.bytes());
}

void IpcCToS::draw_text(int x, int y, const std::string& text,
                        util::font::TextSize size, uint16_t color)
{
    writer_.clear();
    writer_.put_u8       (static_cast<uint8_t>(shared::C2S::DrawText));
    writer_.put_i32      (x);
    writer_.put_i32      (y);
    writer_.put_string   (text);
    writer_.put_text_size(size);
    writer_.put_u16      (color);
    sender_->send(writer_.bytes());
}

void IpcCToS::render()
{
    writer_.clear();
    writer_.put_u8(static_cast<uint8_t>(shared::C2S::Render));
    sender_->send(writer_.bytes());
}

}
