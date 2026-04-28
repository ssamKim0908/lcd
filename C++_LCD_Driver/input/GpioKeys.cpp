#include "GpioKeys.hpp"
#include "../hal/gpio.hpp"

GpioKeys::GpioKeys(std::unique_ptr<GpioRead> reader)
    : reader(std::move(reader))
{}

GpioKeys::~GpioKeys() = default;

bool GpioKeys::is_pressed(Key key)
{
    InputKey pin = InputKey::K1_PIN;
    switch (key)
    {
        case Key::K1:     pin = InputKey::K1_PIN;     break;
        case Key::K2:     pin = InputKey::K2_PIN;     break;
        case Key::K3:     pin = InputKey::K3_PIN;     break;
        case Key::K4:     pin = InputKey::K4_PIN;     break;
        case Key::R:      pin = InputKey::R_PIN;      break;
        case Key::L:      pin = InputKey::L_PIN;      break;
        case Key::Up:     pin = InputKey::UP_PIN;     break;
        case Key::Down:   pin = InputKey::DOWN_PIN;   break;
        case Key::Left:   pin = InputKey::LEFT_PIN;   break;
        case Key::Right:  pin = InputKey::RIGHT_PIN;  break;
        case Key::Center: pin = InputKey::CENTER_PIN; break;
    }
    return reader->read(pin) == GpioValue::Low;
}
