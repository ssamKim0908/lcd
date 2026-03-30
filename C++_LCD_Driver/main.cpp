/********************************
 *  Seengreat 1.3 Inch LCD Display demo
 *  Author(s):Andy Li from Seengreat
 * ********************/
#include "transport_spi.hpp"
#include "transport_interface.hpp"
#include "pch.h"

/******************************
 * when set image in ROTATE_0:
 * 0------------> x (0~240) 
 * |
 * |
 * |
 * |
 * y (0~240)
 * 
 * */

int main(void)
{
	std::unique_ptr<Fd_Spi> spi_transport = std::make_unique<Fd_Spi>("/dev/spidev0.0", O_WRONLY);
	spi_transport->write({0x00, 0x01, 0x02, 0x03}); // Example data to write to SPI device

	return 0;
}
