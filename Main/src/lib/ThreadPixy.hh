#pragma once

#include <pixy.h>
#include <mutex>

typedef struct Block pixy_block_t;

void __attribute__((noreturn)) pixy_thread();

static std::mutex pixy_mutex;

void pixy_cam_init();
void pixy_cam_get(pixy_block_t *output);
