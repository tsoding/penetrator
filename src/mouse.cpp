#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>
#include "mouse.hpp"

Mouse::Mouse():
    uinputDevice({
            {UI_SET_EVBIT, {EV_REL}},
            {UI_SET_RELBIT, {REL_X, REL_Y}}
        }, Uud("Penetrator Mouse"))
{}

void Mouse::moveTo(int x, int y)
{
    uinputDevice.emit(EV_REL, REL_X, x);
    uinputDevice.emit(EV_REL, REL_Y, y);
    uinputDevice.emit(EV_SYN, SYN_REPORT, 0);
}
