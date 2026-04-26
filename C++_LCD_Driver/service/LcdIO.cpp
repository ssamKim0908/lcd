#include "LcdIO.hpp"
#include "../hal/spi.hpp"
#include "../hal/gpio.hpp"
#include "../utils/time.hpp"

LcdReadService::LcdReadService(std::unique_ptr<GpioRead> gpio_read)
    : gpio_read(std::move(gpio_read))
{
    init();
}

LcdReadService::~LcdReadService() = default;

bool LcdReadService::init()
{

    return true;
}

void LcdReadService::read(Span<std::byte> buffer)
{
    //gpio_read->read(buffer);
}

LcdWriteService::LcdWriteService(std::unique_ptr<Spi> spi, std::unique_ptr<GpioWrite> gpio_write)
    : spi(std::move(spi)), gpio_write(std::move(gpio_write))
{
    init();
}

LcdWriteService::~LcdWriteService() = default;

bool LcdWriteService::init()
{
    using namespace utils::time;

    lcd_reset();

    std::byte b{};

    b = std::byte{0x11}; spi_write_cmd ({&b, 1});          // sleep out
    sleep_ms(1000);

    b = std::byte{0x36}; spi_write_cmd ({&b, 1});
    b = std::byte{0x60}; spi_write_data({&b, 1});          // MY=0,MX=1,MV=1,ML=0,RGB Order
    sleep_ms(50);
    b = std::byte{0x3A}; spi_write_cmd ({&b, 1});
    b = std::byte{0x05}; spi_write_data({&b, 1});          // 65k-colors

    b = std::byte{0xB2}; spi_write_cmd ({&b, 1});          // PORCTRL(Porch Setting)
    b = std::byte{0x0C}; spi_write_data({&b, 1});
    b = std::byte{0x0C}; spi_write_data({&b, 1});
    b = std::byte{0x00}; spi_write_data({&b, 1});
    b = std::byte{0x33}; spi_write_data({&b, 1});
    b = std::byte{0x33}; spi_write_data({&b, 1});

    b = std::byte{0xB7}; spi_write_cmd ({&b, 1});          // GCTRL:Gate control
    b = std::byte{0x35}; spi_write_data({&b, 1});

    b = std::byte{0xBB}; spi_write_cmd ({&b, 1});          // VCOMS:VCOM Setting
    b = std::byte{0x37}; spi_write_data({&b, 1});

    b = std::byte{0xC0}; spi_write_cmd ({&b, 1});          // LCMCTRL:LCM Control
    b = std::byte{0x2C}; spi_write_data({&b, 1});

    b = std::byte{0xC2}; spi_write_cmd ({&b, 1});          // VDVVRHEN:VDV and VRH Command Enable
    b = std::byte{0x01}; spi_write_data({&b, 1});

    b = std::byte{0xC3}; spi_write_cmd ({&b, 1});          // VRHS:VRH Set
    b = std::byte{0x12}; spi_write_data({&b, 1});

    b = std::byte{0xC4}; spi_write_cmd ({&b, 1});          // VDVS:VDV Set
    b = std::byte{0x20}; spi_write_data({&b, 1});

    b = std::byte{0xC6}; spi_write_cmd ({&b, 1});          // FRCTRL2:Frame Rate Control in Normal Mode
    b = std::byte{0x0F}; spi_write_data({&b, 1});          // 60Hz

    b = std::byte{0xD0}; spi_write_cmd ({&b, 1});          // PWCTRL1:Power Control 1
    b = std::byte{0xA4}; spi_write_data({&b, 1});
    b = std::byte{0xA1}; spi_write_data({&b, 1});

    b = std::byte{0xE0}; spi_write_cmd ({&b, 1});          // Positive Voltage Gamma Control
    b = std::byte{0xD0}; spi_write_data({&b, 1});
    b = std::byte{0x04}; spi_write_data({&b, 1});
    b = std::byte{0x0D}; spi_write_data({&b, 1});
    b = std::byte{0x11}; spi_write_data({&b, 1});
    b = std::byte{0x13}; spi_write_data({&b, 1});
    b = std::byte{0x2B}; spi_write_data({&b, 1});
    b = std::byte{0x3F}; spi_write_data({&b, 1});
    b = std::byte{0x54}; spi_write_data({&b, 1});
    b = std::byte{0x4C}; spi_write_data({&b, 1});
    b = std::byte{0x18}; spi_write_data({&b, 1});
    b = std::byte{0x0D}; spi_write_data({&b, 1});
    b = std::byte{0x0B}; spi_write_data({&b, 1});
    b = std::byte{0x1F}; spi_write_data({&b, 1});
    b = std::byte{0x23}; spi_write_data({&b, 1});

    b = std::byte{0xE1}; spi_write_cmd ({&b, 1});          // NVGAMCTRL:Negative Voltage Gamma Control
    b = std::byte{0xD4}; spi_write_data({&b, 1});
    b = std::byte{0x04}; spi_write_data({&b, 1});
    b = std::byte{0x0C}; spi_write_data({&b, 1});
    b = std::byte{0x11}; spi_write_data({&b, 1});
    b = std::byte{0x13}; spi_write_data({&b, 1});
    b = std::byte{0x2C}; spi_write_data({&b, 1});
    b = std::byte{0x3F}; spi_write_data({&b, 1});
    b = std::byte{0x44}; spi_write_data({&b, 1});
    b = std::byte{0x51}; spi_write_data({&b, 1});
    b = std::byte{0x2F}; spi_write_data({&b, 1});
    b = std::byte{0x1F}; spi_write_data({&b, 1});
    b = std::byte{0x1F}; spi_write_data({&b, 1});
    b = std::byte{0x20}; spi_write_data({&b, 1});
    b = std::byte{0x23}; spi_write_data({&b, 1});

    b = std::byte{0x21}; spi_write_cmd ({&b, 1});
    sleep_ms(120);
    b = std::byte{0x29}; spi_write_cmd ({&b, 1});
    sleep_ms(50);

    return true;
}

void LcdWriteService::lcd_reset()
{
    using namespace utils::time;
    sleep_ms(100);
    gpio_write->write_pin(OutputKey::PIN_RST, GpioValue::Low);
    sleep_ms(100);
    gpio_write->write_pin(OutputKey::PIN_RST, GpioValue::High);
    sleep_ms(100);
}

void LcdWriteService::spi_write_cmd(Span<const std::byte> cmd)
{
    gpio_write->write_cmd();
    spi->write(cmd);
}

void LcdWriteService::spi_write_data(Span<const std::byte> buffer)
{
    gpio_write->write_data();
    spi->write(buffer);
}

void LcdWriteService::write(Span<const std::byte> buffer)
{
    spi_write_data(buffer);
}
