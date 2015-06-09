#pragma once

typedef struct {
	double x, y;
} camera_point;

typedef struct {
	camera_point top_left, bottom_right;
} camera_data;
