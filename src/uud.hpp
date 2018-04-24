#ifndef UUD_HPP_
#define UUD_HPP_

#include <string>
#include <linux/uinput.h>

class Uud
{
public:
    Uud(const std::string &name);

    void writeTo(int fd) const;

private:
    struct uinput_user_dev uud;
};

#endif  // UUD_HPP_
