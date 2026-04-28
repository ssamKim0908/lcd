#include "include/common.h"
#include "hal/spi.hpp"
#include "hal/gpio.hpp"
#include "display/LcdWriter.hpp"
#include "display/St7789Lcd.hpp"
#include "util/time.hpp"
#include "util/color.hpp"

int main()
{
    using namespace util::time;
    using namespace util::color;

    auto spi    = std::make_unique<Spi> ("/dev/spidev0.0", O_WRONLY);
    auto chip   = std::make_unique<Gpio>("/dev/gpiochip0", O_RDWR);
    auto gw     = std::make_unique<GpioWrite>(chip->get_fd());

    auto writer = std::make_unique<LcdWriter>(std::move(spi), std::move(gw));
    auto lcd    = std::make_unique<St7789Lcd>(std::move(writer));

    lcd->init();

    lcd->clear(RED);   sleep_ms(1000);
    lcd->clear(GREEN); sleep_ms(1000);
    lcd->clear(BLUE);  sleep_ms(1000);

    return 0;
}
