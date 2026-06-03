#pragma once
#include "../../../interface/IProxyClient.hpp"
#include "../../../util/IpcSerializer.hpp"
#include <memory>

class ISender;

namespace sdk::internal
{

class IpcCToS : public IProxyClientToServer
{
private:
    std::shared_ptr<ISender>  sender_;
    util::ipc::Writer         writer_;

public:
    explicit IpcCToS(std::shared_ptr<ISender> sender);
    ~IpcCToS() override;

    IpcCToS(const IpcCToS&)            = delete;
    IpcCToS& operator=(const IpcCToS&) = delete;

    void clear      (uint16_t color)                                                    override;
    void fill_rect  (int x, int y, int w, int h, uint16_t color)                        override;
    void draw_rect  (int x, int y, int w, int h, uint16_t color)                        override;
    void fill_circle(int cx, int cy, int r, uint16_t color)                             override;
    void draw_circle(int cx, int cy, int r, uint16_t color)                             override;
    void draw_text  (int x, int y, const std::string& text, util::font::TextSize size,
                     uint16_t color)                                                    override;
    void render     ()                                                                  override;
};

}
