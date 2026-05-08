#pragma once
#include "TextSize.hpp"
#include <cstdint>
#include <memory>
#include <string_view>

namespace sdk
{

namespace internal { class IpcClient; }

class Draw
{
public:
    explicit Draw(std::shared_ptr<internal::IpcClient> ipc);
    ~Draw();

    Draw(const Draw&)            = delete;
    Draw& operator=(const Draw&) = delete;

    void clear      (uint16_t color);
    void fill_rect  (int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color);
    void draw_rect  (int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color);
    void fill_circle(int32_t cx, int32_t cy, int32_t r, uint16_t color);
    void draw_circle(int32_t cx, int32_t cy, int32_t r, uint16_t color);
    void draw_text  (int32_t x, int32_t y, util::font::TextSize size,
                     uint16_t color, std::string_view text);
    void render     ();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}
