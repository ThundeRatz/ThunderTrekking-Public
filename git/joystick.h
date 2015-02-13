#include <linux/joystick.h>

int joystick_open(char *dev);
void joystick_dump(int fd);
int joystick_read(int fd, struct js_event *event);
