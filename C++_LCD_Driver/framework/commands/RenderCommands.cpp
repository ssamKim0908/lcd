#include "RenderCommands.hpp"
#include "../Rasterizer.hpp"
#include <utility>

ClearCommand::ClearCommand(std::shared_ptr<Rasterizer> receiver, uint16_t color)
    : receiver(std::move(receiver)), color(color) {}
ClearCommand::~ClearCommand() = default;
void ClearCommand::execute()
{
    receiver->clear(color);
}

FillRectCommand::FillRectCommand(std::shared_ptr<Rasterizer> receiver,
                                 int x, int y, int w, int h, uint16_t color)
    : receiver(std::move(receiver)), x(x), y(y), w(w), h(h), color(color) {}
FillRectCommand::~FillRectCommand() = default;
void FillRectCommand::execute()
{
    receiver->fill_rect(x, y, w, h, color);
}

DrawRectCommand::DrawRectCommand(std::shared_ptr<Rasterizer> receiver,
                                 int x, int y, int w, int h, uint16_t color)
    : receiver(std::move(receiver)), x(x), y(y), w(w), h(h), color(color) {}
DrawRectCommand::~DrawRectCommand() = default;
void DrawRectCommand::execute()
{
    receiver->draw_rect(x, y, w, h, color);
}

FillCircleCommand::FillCircleCommand(std::shared_ptr<Rasterizer> receiver,
                                     int cx, int cy, int r, uint16_t color)
    : receiver(std::move(receiver)), cx(cx), cy(cy), r(r), color(color) {}
FillCircleCommand::~FillCircleCommand() = default;
void FillCircleCommand::execute()
{
    receiver->fill_circle(cx, cy, r, color);
}

DrawCircleCommand::DrawCircleCommand(std::shared_ptr<Rasterizer> receiver,
                                     int cx, int cy, int r, uint16_t color)
    : receiver(std::move(receiver)), cx(cx), cy(cy), r(r), color(color) {}
DrawCircleCommand::~DrawCircleCommand() = default;
void DrawCircleCommand::execute()
{
    receiver->draw_circle(cx, cy, r, color);
}

DrawTextCommand::DrawTextCommand(std::shared_ptr<Rasterizer> receiver,
                                 int x, int y,
                                 std::string          text,
                                 util::font::TextSize size,
                                 uint16_t             color)
    : receiver(std::move(receiver)), x(x), y(y),
      text(std::move(text)), size(size), color(color) {}
DrawTextCommand::~DrawTextCommand() = default;
void DrawTextCommand::execute()
{
    receiver->draw_text(x, y, text, size, color);
}

RenderCommand::RenderCommand(std::shared_ptr<Rasterizer> receiver)
    : receiver(std::move(receiver)) {}
RenderCommand::~RenderCommand() = default;
void RenderCommand::execute()
{
    receiver->render();
}
