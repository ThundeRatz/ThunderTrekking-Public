#pragma once

void __attribute__((noreturn)) motors_thread();
void motor(int left, int right);
void set_max_speed(int new_max_speed);
void reset_max_speed();
int get_max_speed();
