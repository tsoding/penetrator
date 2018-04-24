#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "uinput.hpp"

UInput::UInput(const Bits &bits, const Uud &uud):
    fd(0)
{
    // TODO: fd is not closed in case of exception
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd == -1) {
        throw std::runtime_error("Could not open /dev/uinput");
    }

    for (const auto &row: bits) {
        for (const auto &bit: row.second) {
            if (ioctl(fd, row.first, bit) == -1) {
                throw std::runtime_error("Could not set up a bit");
            }
        }
    }

    uud.writeTo(fd);

    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        throw std::runtime_error("Could not create the Penetrator device");
    }
}

UInput::~UInput()
{
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

void UInput::emit(int type, int code, int val)
{
    struct input_event ie;

    memset(&ie, 0, sizeof(ie));
    ie.type = type;
    ie.code = code;
    ie.value = val;

    write(fd, &ie, sizeof(ie));
}
