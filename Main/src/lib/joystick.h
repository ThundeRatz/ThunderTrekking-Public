#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <linux/joystick.h>

int joystick_open(char *dev);
int joystick_open_nb(char *dev);
void joystick_dump(int fd);
int joystick_read(int fd, struct js_event *event);
int joystick_read_nb(int fd, struct js_event *event);

#endif
