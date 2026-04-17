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

    std::vector<long long> x{1, 2, 3, 4, 5};

    controller.read(x);
    controller.write(x);
    return 0;
}
