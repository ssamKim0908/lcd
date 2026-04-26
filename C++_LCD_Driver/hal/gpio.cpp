#include "gpio.hpp"
#include <linux/gpio.h>

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
        ::close(fd);
    }
}

// GpioRead implementation
GpioRead::GpioRead(int gpio_fd)
{
#ifdef TARGET_DEVICE
    struct gpiohandle_request req;
    memset(&req, 0, sizeof(req));

    // Requesting all input pins defined in InputKey enum
    req.lineoffsets[0]  = static_cast<uint32_t>(InputKey::K1_PIN);
    req.lineoffsets[1]  = static_cast<uint32_t>(InputKey::K2_PIN);
    req.lineoffsets[2]  = static_cast<uint32_t>(InputKey::K3_PIN);
    req.lineoffsets[3]  = static_cast<uint32_t>(InputKey::K4_PIN);
    req.lineoffsets[4]  = static_cast<uint32_t>(InputKey::R_PIN);
    req.lineoffsets[5]  = static_cast<uint32_t>(InputKey::L_PIN);
    req.lineoffsets[6]  = static_cast<uint32_t>(InputKey::UP_PIN);
    req.lineoffsets[7]  = static_cast<uint32_t>(InputKey::LEFT_PIN);
    req.lineoffsets[8]  = static_cast<uint32_t>(InputKey::DOWN_PIN);
    req.lineoffsets[9]  = static_cast<uint32_t>(InputKey::RIGHT_PIN);
    req.lineoffsets[10] = static_cast<uint32_t>(InputKey::CENTER_PIN);
    
    req.lines = 11;
    req.flags = GPIOHANDLE_REQUEST_INPUT;
    strncpy(req.consumer_label, "lcd_input_keys", sizeof(req.consumer_label));

    if (ioctl(gpio_fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0)
    {
        throw std::runtime_error("Failed to request GPIO input lines");
    }
    fd = req.fd;
    std::cout << "GPIO input line handle opened, fd: " << fd << std::endl;
#endif
}

GpioRead::~GpioRead()
{
    if (fd >= 0)
    {
        std::cout << "Closing GpioRead line fd: " << fd << std::endl;
        ::close(fd);
    }
}

void GpioRead::read(GpioValue buffer)
{
#ifdef TARGET_DEVICE
    struct gpiohandle_data data;
    if (ioctl(fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0)
    {
        throw std::runtime_error("Failed to read GPIO line values");
    }
    
    for (size_t i = 0; i < buffer.size() && i < 11; ++i) {
        buffer[i] = static_cast<std::byte>(data.values[i]);
    }
#else
    (void)buffer;
    std::cout << "Simulating GPIO read from fd: " << fd << std::endl;
#endif
}

// GpioWrite implementation
GpioWrite::GpioWrite(int gpio_fd)
{
#ifdef TARGET_DEVICE
    struct gpiohandle_request req;
    memset(&req, 0, sizeof(req));

    // Requesting all output pins defined in OutputKey enum
    req.lineoffsets[0] = static_cast<uint32_t>(OutputKey::PIN_RST);
    req.lineoffsets[1] = static_cast<uint32_t>(OutputKey::PIN_DC);
    req.lineoffsets[2] = static_cast<uint32_t>(OutputKey::PIN_BL);
    
    req.lines = 3;
    req.flags = GPIOHANDLE_REQUEST_OUTPUT;
    strncpy(req.consumer_label, "lcd_output_control", sizeof(req.consumer_label));
    memset(req.default_values, 0, sizeof(req.default_values));

    if (ioctl(gpio_fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0)
    {
        throw std::runtime_error("Failed to request GPIO output lines");
    }
    fd = req.fd;
    std::cout << "GPIO output line handle opened, fd: " << fd << std::endl;
#endif
}

GpioWrite::~GpioWrite()
{
    if (fd >= 0)
    {
        std::cout << "Closing GpioWrite line fd: " << fd << std::endl;
        ::close(fd);
    }
}

void GpioWrite::write_cmd()
{
#ifdef TARGET_DEVICE
    
#else
    //(void)buffer;
    std::cout << "Simulating GPIO write to fd: " << fd << std::endl;
#endif
}

void GpioWrite::write_data()
{
#ifdef TARGET_DEVICE
#else
#endif
}

void GpioWrite::write_pin(OutputKey pin, GpioValue value)
{
#ifdef TARGET_DEVICE
#else
#endif
}