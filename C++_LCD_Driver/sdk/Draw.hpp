#pragma once
#include "../interface/IDrawContract.hpp"
#include <cstdint>
#include <memory>
#include <string>

namespace sdk
{

namespace internal { class IpcClient; }

class Draw : public IDrawContract
{
public:
    explicit Draw(std::shared_ptr<internal::IpcClient> ipc);
    ~Draw() override;

    Draw(const Draw&)            = delete;
    Draw& operator=(const Draw&) = delete;

    void clear      (uint16_t color) override;
    void fill_rect  (int x, int y, int w, int h, uint16_t color) override;
    void draw_rect  (int x, int y, int w, int h, uint16_t color) override;
    void fill_circle(int cx, int cy, int r, uint16_t color) override;
    void draw_circle(int cx, int cy, int r, uint16_t color) override;
    void draw_text  (int x, int y, const std::string& text, util::font::TextSize size,
                     uint16_t color) override;
    void render     () override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}
