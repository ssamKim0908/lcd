/********************************
 *  Seengreat 1.3 Inch LCD Display demo
 *  Author(s):Andy Li from Seengreat
 * ********************/
#include "include/pch.h"
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
    auto factory = std::make_shared<LcdFactoryCommunication>();
    Controller controller(factory);
    return 0;
}
