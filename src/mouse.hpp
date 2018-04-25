#ifndef MOUSE_HPP_
#define MOUSE_HPP_

#include "uinput.hpp"

class Mouse
{
public:
    Mouse();

    void moveTo(int x, int y);

private:
    UInput uinputDevice;
};

#endif  // MOUSE_HPP_
