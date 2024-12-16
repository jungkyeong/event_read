#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <linux/input.h>

// #include "wheel_test.h"

#define INPUT_EVENT "/dev/input/event4"

#define TYPE_BTN_LEFT_RIGTH 16
#define TYPE_BTN_TOP_BOT 17
#define TYPE_AXCEL 2
#define TYPE_BRAKE 5
#define TYPE_CLUTCH 1
#define TYPE_HANDLE 0

int main() {

    const char *dev = INPUT_EVENT;

    // file device open read version
    int fd = open(dev, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        return 1;
    }

    struct input_event ev; // define open device structure (time, type, code, value)

    printf("Reading events from %s...\n", dev);

    while (1) {
        // read event input
        int status = read(fd, &ev, sizeof(struct input_event));
        if (status == -1) {
            if (errno == EINTR) { // Mabye Catch Interrupt system calling, system recall
                continue;
            }
            perror("Error reading input event");
            close(fd);
            return 1;
        }

        // event read print
        if (status == sizeof(struct input_event)) {
            printf("Time: %ld.%06ld, Type: %d, Code: %d, Value: %d\n",
                   ev.time.tv_sec, ev.time.tv_usec,
                   ev.type, ev.code, ev.value);
        }

        if(ev.type == EV_KEY){
            printf("Button %d, Value %d \n", ev.code, ev.value);
        }
        else if(ev.type == EV_ABS){
            if(ev.code == TYPE_HANDLE){
                printf("Handle %d, Value %d \n", ev.code, ev.value);
            }
            else if(ev.code == TYPE_CLUTCH){
                printf("Clutch %d, Value %d \n", ev.code, ev.value);
            }
            else if(ev.code == TYPE_BRAKE){
                printf("Brake %d, Value %d \n", ev.code, ev.value);
            }
            else if(ev.code == TYPE_AXCEL){
                printf("Axcel %d, Value %d \n", ev.code, ev.value);
            }
            else if(ev.code == TYPE_BTN_TOP_BOT){
                if(ev.value == 1){
                    printf("Down %d, Value %d \n", ev.code, ev.value);   
                }
                else if(ev.value == -1){
                    printf("UP %d, Value %d \n", ev.code, ev.value);   
                }
            }
            else if(ev.code == TYPE_BTN_LEFT_RIGTH){
                if(ev.value == 1){
                    printf("RIGHT %d, Value %d \n", ev.code, ev.value);   
                }
                else if(ev.value == -1){
                    printf("LEFT %d, Value %d \n", ev.code, ev.value);   
                }
            }
        }
    }

    close(fd);
    return 0;
}