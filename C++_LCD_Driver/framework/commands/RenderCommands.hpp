#pragma once
#include "../../interface/IRenderCommand.hpp"
#include "../../util/Font.hpp"
#include <memory>
#include <string>
#include <cstdint>

class Rasterizer;

class ClearCommand : public IRenderCommand
{
private:
    std::shared_ptr<Rasterizer> receiver;
    uint16_t                    color;
public:
    ClearCommand(std::shared_ptr<Rasterizer> receiver, uint16_t color);
    ~ClearCommand() override;
    void execute() override;
};

class FillRectCommand : public IRenderCommand
{
private:
    std::shared_ptr<Rasterizer> receiver;
    int                         x, y, w, h;
    uint16_t                    color;
public:
    FillRectCommand(std::shared_ptr<Rasterizer> receiver,
                    int x, int y, int w, int h, uint16_t color);
    ~FillRectCommand() override;
    void execute() override;
};

class DrawRectCommand : public IRenderCommand
{
private:
    std::shared_ptr<Rasterizer> receiver;
    int                         x, y, w, h;
    uint16_t                    color;
public:
    DrawRectCommand(std::shared_ptr<Rasterizer> receiver,
                    int x, int y, int w, int h, uint16_t color);
    ~DrawRectCommand() override;
    void execute() override;
};

class FillCircleCommand : public IRenderCommand
{
private:
    std::shared_ptr<Rasterizer> receiver;
    int                         cx, cy, r;
    uint16_t                    color;
public:
    FillCircleCommand(std::shared_ptr<Rasterizer> receiver,
                      int cx, int cy, int r, uint16_t color);
    ~FillCircleCommand() override;
    void execute() override;
};

class DrawCircleCommand : public IRenderCommand
{
private:
    std::shared_ptr<Rasterizer> receiver;
    int                         cx, cy, r;
    uint16_t                    color;
public:
    DrawCircleCommand(std::shared_ptr<Rasterizer> receiver,
                      int cx, int cy, int r, uint16_t color);
    ~DrawCircleCommand() override;
    void execute() override;
};

class DrawTextCommand : public IRenderCommand
{
private:
    std::shared_ptr<Rasterizer> receiver;
    int                         x, y;
    std::string                 text;
    util::font::TextSize        size;
    uint16_t                    color;
public:
    DrawTextCommand(std::shared_ptr<Rasterizer> receiver,
                    int x, int y,
                    std::string          text,
                    util::font::TextSize size,
                    uint16_t             color);
    ~DrawTextCommand() override;
    void execute() override;
};

class RenderCommand : public IRenderCommand
{
private:
    std::shared_ptr<Rasterizer> receiver;
public:
    explicit RenderCommand(std::shared_ptr<Rasterizer> receiver);
    ~RenderCommand() override;
    void execute() override;
};
