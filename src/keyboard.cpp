#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <vector>

#include "./keyboard.hpp"

namespace
{
    const std::vector<int> keyboard_letters = {
        KEY_A, KEY_B, KEY_C,
        KEY_D, KEY_E, KEY_F,
        KEY_G, KEY_H, KEY_I,
        KEY_J, KEY_K, KEY_L,
        KEY_M, KEY_N, KEY_O,
        KEY_P, KEY_Q, KEY_R,
        KEY_S, KEY_T, KEY_U,
        KEY_V, KEY_W, KEY_X,
        KEY_Y, KEY_Z
    };

    int char_to_code(char x)
    {
        return keyboard_letters[x - 'a'];
    }
}

Keyboard::Keyboard():
    uinputDevice({
            {UI_SET_EVBIT, {EV_KEY}},
            {UI_SET_KEYBIT, keyboard_letters}
        },
        Uud("Penetrator Keyboard"))
{}

void Keyboard::injectString(const std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i) {
        if (isalpha(str[i]) && islower(str[i])) {
            const int code = char_to_code(str[i]);
            uinputDevice.emit(EV_KEY, code, 1);
            uinputDevice.emit(EV_SYN, SYN_REPORT, 0);
            uinputDevice.emit(EV_KEY, code, 0);
            uinputDevice.emit(EV_SYN, SYN_REPORT, 0);
        }
    }
}
