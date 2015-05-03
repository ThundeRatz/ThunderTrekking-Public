#ifndef __PIXY_CAM_H__
#define __PIXY_CAM_H__

#include <pixy.h>

typedef struct Block pixy_block_t;

void pixy_cam_init();
void pixy_cam_get(pixy_block_t *output);

#endif
