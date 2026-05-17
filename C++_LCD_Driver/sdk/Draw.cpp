#include "Draw.hpp"
#include "internal/IpcClient.hpp"
#include "../shared/MsgType.hpp"
#include "../interface/IProxyClientToServer.hpp"

namespace sdk
{

struct Draw::Impl
{
    std::unique_ptr<IProxyClientToServer> proxy;
};

Draw::Draw()
    : impl_(std::make_unique<Impl>())
{
    impl_->proxy; 
}

Draw::~Draw() = default;

void Draw::clear(uint16_t color)
{
    impl_->proxy->clear(color);
}

void Draw::fill_rect(int x, int y, int w, int h, uint16_t color)
{
    impl_->proxy->fill_rect(x, y, w, h, color);
}

void Draw::draw_rect(int x, int y, int w, int h, uint16_t color)
{
    impl_->proxy->draw_rect(x, y, w, h, color);
}

void Draw::fill_circle(int cx, int cy, int r, uint16_t color)
{
    impl_->proxy->fill_circle(cx, cy, r, color);
}

void Draw::draw_circle(int cx, int cy, int r, uint16_t color)
{
    impl_->proxy->draw_circle(cx, cy, r, color);
}

void Draw::draw_text(int x, int y, const std::string& text, util::font::TextSize size,
                     uint16_t color)
{
    impl_->proxy->draw_text(x, y, text, size, color);
}

void Draw::render()
{
    impl_->proxy->render();
}

}
