#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>

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
const size_t keyboard_letters_size = sizeof(keyboard_letters) / sizeof(int);

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

void emit_string(int fd, const char *str)
{
    const size_t n = strlen(str);

    for (size_t i = 0; i < n; ++i) {
        if (isalpha(str[i]) && islower(str[i])) {
            const int code = char_to_code(str[i]);
            emit(fd, EV_KEY, code, 1);
            emit(fd, EV_SYN, SYN_REPORT, 0);
            emit(fd, EV_KEY, code, 0);
            emit(fd, EV_SYN, SYN_REPORT, 0);
        }
    }
}

int main(int argc, char *argv[])
{
    struct uinput_user_dev uud;

    if (argc < 3) {
        fprintf(stderr, "Usage: ./penetrator keyboard <string>\n");
        return -1;
    }

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd == -1) {
        perror("Could not open /dev/uinput");
        return -1;
    }

    if (ioctl(fd, UI_SET_EVBIT, EV_KEY) == -1) {
        perror("Could not enable EV_KEY");
        return -1;
    }

    for (size_t i = 0; i < keyboard_letters_size; ++i) {
        if (ioctl(fd, UI_SET_KEYBIT, keyboard_letters[i]) == -1) {
            perror("Could not enable keys");
            return -1;
        }
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

    emit_string(fd, argv[2]);

    sleep(1);

    ioctl(fd, UI_DEV_DESTROY);

    close(fd);

    return 0;
}
