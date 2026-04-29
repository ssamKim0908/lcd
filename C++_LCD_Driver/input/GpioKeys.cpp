#include "GpioKeys.hpp"
#include "../hal/gpio.hpp"


GpioKeys::GpioKeys(std::unique_ptr<GpioRead> reader)
    : reader(std::move(reader))
{
    static const std::map<InputKey, Key> key_map = {
        { InputKey::K1_PIN,     Key::K1     },
        { InputKey::K2_PIN,     Key::K2     },
        { InputKey::K3_PIN,     Key::K3     },
        { InputKey::K4_PIN,     Key::K4     },
        { InputKey::R_PIN,      Key::R      },
        { InputKey::L_PIN,      Key::L      },
        { InputKey::UP_PIN,     Key::Up     },
        { InputKey::DOWN_PIN,   Key::Down   },
        { InputKey::LEFT_PIN,   Key::Left   },
        { InputKey::RIGHT_PIN,  Key::Right  },
        { InputKey::CENTER_PIN, Key::Center },
    };
}

GpioKeys::~GpioKeys() = default;

KeyEvent GpioKeys::next_event()
{
    auto ev = reader->wait_event();
    return { key_map.at(ev.key), ev.pressed ? KeyState::Pressed : KeyState::Released};
}
