#include "DrawImpl.hpp"
#include "ipc/IpcCToS.hpp"

#include <utility>

namespace sdk
{

DrawImpl::DrawImpl(std::unique_ptr<IProxyClientToServer> proxy)
    : proxy_(std::move(proxy)) {}

DrawImpl::~DrawImpl() = default;

void DrawImpl::clear(uint16_t color)
{
    proxy_->clear(color);
}

void DrawImpl::fill_rect(int x, int y, int w, int h, uint16_t color)
{
    proxy_->fill_rect(x, y, w, h, color);
}

void DrawImpl::draw_rect(int x, int y, int w, int h, uint16_t color)
{
    proxy_->draw_rect(x, y, w, h, color);
}

void DrawImpl::fill_circle(int cx, int cy, int r, uint16_t color)
{
    proxy_->fill_circle(cx, cy, r, color);
}

void DrawImpl::draw_circle(int cx, int cy, int r, uint16_t color)
{
    proxy_->draw_circle(cx, cy, r, color);
}

void DrawImpl::draw_text(int x, int y, const std::string& text,
                         util::font::TextSize size, uint16_t color)
{
    proxy_->draw_text(x, y, text, size, color);
}

void DrawImpl::render()
{
    proxy_->render();
}

}
