#include "gpio.hpp"
#include <cstring>
#include <linux/gpio.h>
#include <cerrno>
#include <system_error>
#include "../include/common.h"

Gpio::Gpio(const std::string& device, int flags)
{
#ifdef TARGET_DEVICE
    fd = ::open(device.c_str(), flags);
    if (fd < 0)
    {
        throw std::runtime_error("Failed to open GPIO device: " + device);
    }
    std::cout << "Opened GPIO fd: " << fd << std::endl;
#else
    (void)device;
    (void)flags;
#endif
}

Gpio::~Gpio()
{
    if (fd >= 0)
    {
        std::cout << "Closing GPIO fd: " << fd << std::endl;
        close(fd);
    }
}

// ─── GpioRead ──────────────────────────────────────────────────────

GpioRead::GpioRead(int gpio_fd)
{
#ifdef TARGET_DEVICE
    constexpr InputKey all_keys[] = {
        InputKey::K1_PIN, InputKey::K2_PIN, InputKey::K3_PIN, InputKey::K4_PIN,
        InputKey::R_PIN,  InputKey::L_PIN,
        InputKey::UP_PIN, InputKey::LEFT_PIN, InputKey::DOWN_PIN, InputKey::RIGHT_PIN,
        InputKey::CENTER_PIN,
    };

    try
    {
        for (auto key : all_keys)
        {
            line_fds[key] = read_request_line(gpio_fd, key);
        }
    }
    catch (...)
    {
        close_all();
        throw;
    }
    std::cout << "GpioRead: opened " << line_fds.size() << " input lines" << std::endl;
#else
    (void)gpio_fd;
#endif
}

GpioRead::~GpioRead()
{
    close_all();
}

void GpioRead::close_all()
{
    for (auto& kv : line_fds)
    {
        if (kv.second >= 0) close(kv.second);
    }
    line_fds.clear();
}

int GpioRead::read_request_line(int chip_fd, InputKey pin)
{
#ifdef TARGET_DEVICE
    struct gpio_v2_line_request req;
    std::memset(&req, 0, sizeof(req));

    req.offsets[0]   = static_cast<uint32_t>(pin);
    req.num_lines    = 1;
    req.config.flags = GPIO_V2_LINE_FLAG_INPUT | GPIO_V2_LINE_FLAG_BIAS_PULL_UP;
    std::strncpy(req.consumer, "lcd_input", sizeof(req.consumer));

    if (ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &req) < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "GPIO_V2_GET_LINE_IOCTL (input)");
    }
    return req.fd;
#else
    (void)chip_fd; (void)pin;
    return -1;
#endif
}

GpioValue GpioRead::read(InputKey pin)
{
#ifdef TARGET_DEVICE
    auto it = line_fds.find(pin);
    if (it == line_fds.end())
    {
        throw std::invalid_argument("GpioRead::read: unknown InputKey");
    }

    struct gpio_v2_line_values vals{};
    vals.mask = 1;
    if (ioctl(it->second, GPIO_V2_LINE_GET_VALUES_IOCTL, &vals) < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "GPIO_V2_LINE_GET_VALUES_IOCTL");
    }
    return (vals.bits & 1ULL) ? GpioValue::High : GpioValue::Low;
#else
    (void)pin;
    return GpioValue::Low;
#endif
}

// ─── GpioWrite ─────────────────────────────────────────────────────

GpioWrite::GpioWrite(int gpio_fd)
{
#ifdef TARGET_DEVICE
    struct PinInit { OutputKey key; GpioValue defv; };
    constexpr PinInit pins[] = {
        { OutputKey::PIN_RST, GpioValue::High },
        { OutputKey::PIN_DC,  GpioValue::Low  },
        { OutputKey::PIN_BL,  GpioValue::High },
    };

    try
    {
        for (auto& p : pins)
        {
            line_fds[p.key] = write_request_line(gpio_fd, p.key, p.defv);
        }
    }
    catch (...)
    {
        close_all();
        throw;
    }
    std::cout << "GpioWrite: opened " << line_fds.size() << " output lines" << std::endl;
#else
    (void)gpio_fd;
#endif
}

GpioWrite::~GpioWrite()
{
    close_all();
}

void GpioWrite::close_all()
{
    for (auto& kv : line_fds)
    {
        if (kv.second >= 0) ::close(kv.second);
    }
    line_fds.clear();
}

int GpioWrite::write_request_line(int chip_fd, OutputKey pin)
{
#ifdef TARGET_DEVICE
    struct gpio_v2_line_request req;
    std::memset(&req, 0, sizeof(req));

    req.offsets[0]   = static_cast<uint32_t>(pin);
    req.num_lines    = 1;
    req.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;
    std::strncpy(req.consumer, "lcd_output", sizeof(req.consumer));

    req.config.num_attrs            = 1;
    req.config.attrs[0].attr.id     = GPIO_V2_LINE_ATTR_ID_OUTPUT_VALUES;
    req.config.attrs[0].attr.values = 1ULL;
    req.config.attrs[0].mask        = 1ULL;

    if (ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &req) < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "GPIO_V2_GET_LINE_IOCTL (output)");
    }
    return req.fd;
#else
    (void)chip_fd; (void)pin;
    return -1;
#endif
}

void GpioWrite::write_pin(OutputKey pin, GpioValue value)
{
#ifdef TARGET_DEVICE
    auto it = line_fds.find(pin);
    if (it == line_fds.end())
    {
        throw std::invalid_argument("GpioWrite::write_pin: unknown OutputKey");
    }

    struct gpio_v2_line_values vals{};
    vals.mask = 1ULL;
    vals.bits = (value == GpioValue::High) ? 1ULL : 0ULL;

    if (ioctl(it->second, GPIO_V2_LINE_SET_VALUES_IOCTL, &vals) < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "GPIO_V2_LINE_SET_VALUES_IOCTL");
    }
#else
    (void)pin; (void)value;
#endif
}

void GpioWrite::write_cmd()
{
    write_pin(OutputKey::PIN_DC, GpioValue::Low);
}

void GpioWrite::write_data()
{
    write_pin(OutputKey::PIN_DC, GpioValue::High);
}
