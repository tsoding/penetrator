#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./keyboard.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: ./penetrator keyboard <string>\n");
        return -1;
    }

    Keyboard keyboard;

    sleep(1);
    keyboard.injectString(argv[2]);
    sleep(1);

    return 0;
}
