/********************************
 *  Seengreat 1.3 Inch LCD Display demo
 *  Author(s):Andy Li from Seengreat
 * ********************/
#include "include/common.h"
#include "lcd/controller.hpp"
#include "factory/communication.hpp"

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
    auto factory = std::make_unique<LcdFactoryCommunication>();
    Controller controller(std::move(factory));
    return 0;
}
