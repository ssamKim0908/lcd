#include "Draw.hpp"
#include "internal/DrawImpl.hpp"

#include <utility>

namespace sdk
{

Draw::Draw(std::unique_ptr<DrawImpl> impl)
    : impl_(std::move(impl)) {}

Draw::~Draw() = default;

void Draw::clear      (uint16_t color)                                          { impl_->clear(color); }
void Draw::fill_rect  (int x, int y, int w, int h, uint16_t color)              { impl_->fill_rect  (x, y, w, h, color); }
void Draw::draw_rect  (int x, int y, int w, int h, uint16_t color)              { impl_->draw_rect  (x, y, w, h, color); }
void Draw::fill_circle(int cx, int cy, int r, uint16_t color)                   { impl_->fill_circle(cx, cy, r, color); }
void Draw::draw_circle(int cx, int cy, int r, uint16_t color)                   { impl_->draw_circle(cx, cy, r, color); }
void Draw::draw_text  (int x, int y, const std::string& text,
                       util::font::TextSize size, uint16_t color)               { impl_->draw_text  (x, y, text, size, color); }
void Draw::render     ()                                                        { impl_->render(); }

}
