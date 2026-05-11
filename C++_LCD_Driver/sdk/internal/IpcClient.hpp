#pragma once
#include "../../shared/KeyEvent.hpp"
#include "../../util/Font.hpp"
#include "../../shared/DrawCommand.hpp"
#include "../../util/span.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

class IChannel;

namespace sdk::internal
{

namespace detail
{

template <typename T>
void put(std::vector<std::byte>& v, const T& x)
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    v.insert(v.end(), p, p + sizeof(T));
}

inline void put(std::vector<std::byte>& v, util::font::TextSize size)
{
    put(v, static_cast<uint8_t>(size));
}

inline void put(std::vector<std::byte>& v, std::string_view text)
{
    put(v, static_cast<uint16_t>(text.size()));
    const auto* p = reinterpret_cast<const std::byte*>(text.data());
    v.insert(v.end(), p, p + text.size());
}

}

class IpcClient
{
public:
    explicit IpcClient(std::unique_ptr<IChannel> channel);
    ~IpcClient();

    IpcClient(const IpcClient&)            = delete;
    IpcClient& operator=(const IpcClient&) = delete;

    template <typename... Ts>
    void send_command(shared::DrawCommand cmd, const Ts&... args)
    {
        std::vector<std::byte> buf;
        detail::put(buf, static_cast<uint8_t>(cmd));
        (detail::put(buf, args), ...);
        send_bytes(util::Span<const std::byte>(buf.data(), buf.size()));
    }

    void     send_bytes(util::Span<const std::byte> data);
    KeyEvent recv_key  ();

private:
    std::unique_ptr<IChannel> channel_;
};

}
