#include "gpio.hpp"
#include <cstring>
#include <linux/gpio.h>
#include <cerrno>
#include <system_error>
#include <unistd.h>
#include <sys/epoll.h>
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

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        throw std::system_error(errno, std::generic_category(), "epoll_create1");
    }

    try
    {
        for (auto key : all_keys)
        {
            int line_fd = read_request_line(gpio_fd, key);
            line_fds[key] = line_fd;

            struct epoll_event ev{};
            ev.events     = EPOLLIN;
            ev.data.u32   = static_cast<uint32_t>(key);
            if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, line_fd, &ev) < 0)
            {
                throw std::system_error(errno, std::generic_category(), "epoll_ctl");
            }
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

    if (epoll_fd >= 0)
    {
        close(epoll_fd);
        epoll_fd = -1;
    }
}

int GpioRead::read_request_line(int chip_fd, InputKey pin)
{
#ifdef TARGET_DEVICE
    struct gpio_v2_line_request req;
    std::memset(&req, 0, sizeof(req));

    req.offsets[0]   = static_cast<uint32_t>(pin);
    req.num_lines    = 1;
    req.config.flags = GPIO_V2_LINE_FLAG_INPUT
                     | GPIO_V2_LINE_FLAG_BIAS_PULL_UP
                     | GPIO_V2_LINE_FLAG_EDGE_FALLING
                     | GPIO_V2_LINE_FLAG_EDGE_RISING;
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

// ─── GpioRead public ─────────────────────────────────────────────────────

GpioEvent GpioRead::wait_event()
{
#ifdef TARGET_DEVICE
    struct epoll_event ev{};
    if (epoll_wait(epoll_fd, &ev, 1, -1) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "epoll_wait");
    }

    InputKey key    = static_cast<InputKey>(ev.data.u32);
    int      line_fd = line_fds.at(key);

    struct gpio_v2_line_event line_ev{};
    if (::read(line_fd, &line_ev, sizeof(line_ev)) < 0)
    {
        throw std::system_error(errno, std::generic_category(), "gpio read event");
    }

    bool pressed = (line_ev.id == GPIO_V2_LINE_EVENT_FALLING_EDGE);
    return { key, pressed };
#else
    return { InputKey::K1_PIN, false };
#endif
}

// ─── GpioWrite private ─────────────────────────────────────────────────────

int GpioWrite::write_request_line(int chip_fd, OutputKey pin, GpioValue default_value)
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
    req.config.attrs[0].attr.values = (default_value == GpioValue::High) ? 1ULL : 0ULL;
    req.config.attrs[0].mask        = 1ULL;

    if (ioctl(chip_fd, GPIO_V2_GET_LINE_IOCTL, &req) < 0)
    {
        throw std::system_error(errno, std::generic_category(),
            "GPIO_V2_GET_LINE_IOCTL (output)");
    }
    return req.fd;
#else
    (void)chip_fd; (void)pin; (void)default_value;
    return -1;
#endif
}

void GpioWrite::close_all()
{
    for (auto& kv : line_fds)
    {
        if (kv.second >= 0) ::close(kv.second);
    }
    line_fds.clear();
}

// ─── GpioWrite public ─────────────────────────────────────────────────────

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

void GpioWrite::write_cmd()
{
    write_pin(OutputKey::PIN_DC, GpioValue::Low);
}

void GpioWrite::write_data()
{
    write_pin(OutputKey::PIN_DC, GpioValue::High);
}
