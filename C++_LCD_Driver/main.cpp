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
	std::unique_ptr<Fd_spi> spi = std::make_unique<Fd_spi>("/dev/spidev0.0", O_RDWR);
	
	return 0;
}
