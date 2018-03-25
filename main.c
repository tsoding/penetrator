#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>

void emit(int fd, int type, int code, int val)
{
    struct input_event ie;

    memset(&ie, 0, sizeof(ie));
    ie.type = type;
    ie.code = code;
    ie.value = val;

    write(fd, &ie, sizeof(ie));
}

int main(int argc, char *argv[])
{
    struct uinput_user_dev uud;

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd == -1) {
        perror("Could not open /dev/uinput");
        return -1;
    }

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) == -1) {
        perror("Could not enable EV_KEY");
        return -1;
    }
    if (ioctl(fd, UI_SET_KEYBIT, KEY_A) == -1) {
        perror("Could not enable KEY_A");
        return -1;
    }

    memset(&uud, 0, sizeof(uud));
    snprintf(uud.name, UINPUT_MAX_NAME_SIZE, "Penetrator Device");
    if (write(fd, &uud, sizeof(uud)) < 0) {
        perror("Could not write to /dev/uinput");
        return -1;
    }

    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        perror("Could not create the Penetrator device");
        return -1;
    }

    sleep(1);

    for (int i = 0; i < 10; ++i) {
        emit(fd, EV_KEY, KEY_A, 1);
        emit(fd, EV_SYN, SYN_REPORT, 0);
        emit(fd, EV_KEY, KEY_A, 0);
        emit(fd, EV_SYN, SYN_REPORT, 0);
    }

    sleep(1);

    ioctl(fd, UI_DEV_DESTROY);

    close(fd);

    return 0;
}
