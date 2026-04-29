#include "include/common.h"
#include "hal/spi.hpp"
#include "hal/gpio.hpp"
#include "display/LcdWriter.hpp"
#include "display/St7789Lcd.hpp"
#include "input/GpioKeys.hpp"
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

    auto input  = std::make_unique<GpioRead>(chip->get_fd());
    auto key    = std::make_unique<GpioKeys>(std::move(input));

    lcd->init();

    auto c = std::vector<uint16_t>({RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA});

    int cur = 1;
    while(cur > 0)
    {
        lcd->clear(c[cur]);
        lcd->render();
        auto k = key->next_event();
        if(k.state == KeyState::Pressed)
        {
            switch(k.key)
            {
            case Key::Left : cur = cur > 0 ? cur - 1 : cur; break;
            case Key::Right: cur = cur < c.size() - 1 ? cur + 1 : cur; break;
            }
        }
    }
    return 0;
}