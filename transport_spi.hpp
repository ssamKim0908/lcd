#include <linux/spi/spidev.h>
#include "transport_abstract.hpp"

class transport_spi : public transport
{
public:
    bool trans_init() override
    {
        ioctl(get_fd(), SPI_IOC_WR_MODE, &mode);
    }
}