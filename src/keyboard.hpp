#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include <string>

class Keyboard
{
public:
    Keyboard();
    ~Keyboard();

    void injectString(const std::string &s);

private:
    int fd;
};

#endif  // KEYBOARD_HPP_
