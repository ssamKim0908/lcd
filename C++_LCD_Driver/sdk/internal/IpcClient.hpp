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
    static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    v.insert(v.end(), p, p + sizeof(T));
}

inline void put(std::vector<std::byte>& v, int32_t x)
{
    const auto* p = reinterpret_cast<const std::byte*>(&x);
    v.insert(v.end(), p, p + 4);
}

inline void put(std::vector<std::byte>& v, int x)
{
    put(v, static_cast<int32_t>(x));
}

inline void put(std::vector<std::byte>& v, util::font::TextSize size)
{
    v.push_back(static_cast<std::byte>(size));
}

inline void put(std::vector<std::byte>& v, std::string_view text)
{
    put(v, static_cast<uint16_t>(text.size()));
    const auto* p = reinterpret_cast<const std::byte*>(text.data());
    v.insert(v.end(), p, p + text.size());
}

inline void put(std::vector<std::byte>& v, const std::string& text)
{
    put(v, std::string_view(text));
}

inline void put(std::vector<std::byte>& v, const char* text)
{
    put(v, std::string_view(text));
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
