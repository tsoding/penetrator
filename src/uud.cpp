#include <cstdlib>
#include <cstring>

#include <stdexcept>

#include <fcntl.h>
#include <unistd.h>
#include "uud.hpp"

Uud::Uud(const std::string &name)
{
    memset(&uud, 0, sizeof(uud));
    snprintf(uud.name, UINPUT_MAX_NAME_SIZE, name.c_str());
}

void Uud::writeTo(int fd) const
{
    if (write(fd, &uud, sizeof(uud)) < 0) {
        throw std::runtime_error("Could not write to /dev/uinput");
    }
}
