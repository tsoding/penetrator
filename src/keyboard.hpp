#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include <string>
#include "uinput.hpp"

class Keyboard
{
public:
    Keyboard();

    void injectString(const std::string &s);

private:
    UInput uinputDevice;
};

#endif  // KEYBOARD_HPP_
