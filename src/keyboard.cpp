#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>

#include "./keyboard.hpp"

namespace
{
    const int keyboard_letters[] = {
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

    const size_t keyboard_letters_size =
        sizeof(keyboard_letters) / sizeof(int);

    int char_to_code(char x)
    {
        return keyboard_letters[x - 'a'];
    }

    void emit(int fd, int type, int code, int val)
    {
        struct input_event ie;

        memset(&ie, 0, sizeof(ie));
        ie.type = type;
        ie.code = code;
        ie.value = val;

        write(fd, &ie, sizeof(ie));
    }
}

Keyboard::Keyboard():
    fd(0)
{
    struct uinput_user_dev uud;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd == -1) {
        throw std::runtime_error("Could not open /dev/uinput");
    }

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) == -1) {
        throw std::runtime_error("Could not enable EV_KEY");
    }

    for (size_t i = 0; i < keyboard_letters_size; ++i) {
        if (ioctl(fd, UI_SET_KEYBIT, keyboard_letters[i]) == -1) {
            throw std::runtime_error("Could not enable keys");
        }
    }

    memset(&uud, 0, sizeof(uud));
    snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "Penetrator Device");
    if (write(fd, &uud, sizeof(uud)) < 0) {
        throw std::runtime_error("Could not write to /dev/uinput");
    }

    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        throw std::runtime_error("Could not create the Penetrator device");
    }
}

Keyboard::~Keyboard()
{
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

void Keyboard::injectString(const std::string &str)
{
    for (size_t i = 0; i < str.size(); ++i) {
        if (isalpha(str[i]) && islower(str[i])) {
            const int code = char_to_code(str[i]);
            emit(fd, EV_KEY, code, 1);
            emit(fd, EV_SYN, SYN_REPORT, 0);
            emit(fd, EV_KEY, code, 0);
            emit(fd, EV_SYN, SYN_REPORT, 0);
        }
    }
}
