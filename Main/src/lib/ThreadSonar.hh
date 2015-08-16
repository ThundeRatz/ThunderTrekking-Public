#pragma once

#include <cstdint>

extern uint64_t sonar_l, sonar_r;
void __attribute__((noreturn)) sonar_thread();
