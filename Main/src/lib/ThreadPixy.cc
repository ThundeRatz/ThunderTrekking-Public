#include <iostream>
#include <stdlib.h>
#include <cstdint>
#include <cstring>
#include <thread>

#include "ThreadSpawn.hh"
#include "ThreadPixy.hh"

#define len(array)	((&array)[1] - array)
#define MS			1000000
#define status_perror(__msg, __errno)	do {if ((__errno)) {char __errmsg[50]; fprintf(stderr, "%s: %s\n", __msg, strerror_r(__errno, __errmsg, sizeof(__errmsg)));}} while (0)
#define status_try(cmd, errmsg)			do {int __errno = (cmd); if (__errno) status_perror(errmsg, __errno);} while (0)

using namespace std;

static pixy_block_t blocks[100];
static pixy_block_t largest_block;

void pixy_cam_init() {
	int error_code;
	if ((error_code = pixy_init())) {
		pixy_error(error_code);
		exit(-1);
	}

#ifdef DEBUG
	uint16_t major, minor, build;
	if ((error_code = pixy_get_firmware_version(&major, &minor, &build)))
		pixy_error(error_code);
	else
		printf("Pixy firmware %hu.%hu.%hu\n", major, minor, build);
#endif

	thread_spawn(pixy_thread);
}

static void pixy_clear() {
	memset(&largest_block, 0, sizeof(largest_block));
}

void __attribute__((noreturn)) pixy_thread() {
	int number_objects, i, largest, largest_size;
	const struct timespec block_wait_time = {.tv_sec = 0, .tv_nsec = 25 * MS};
	for (;;) {
		if (nanosleep(&block_wait_time, NULL))
			perror("nanosleep");

		if (pixy_blocks_are_new()) {
			number_objects = pixy_get_blocks(len(blocks), blocks);

			if (!number_objects)
				pixy_clear();
			else {
				if (number_objects < 0) {
					pixy_error(number_objects);
					continue;
				}

#ifdef PIXY_DEBUG
				printf("PIXY: %d blocks received\n", number_objects);
#endif

				largest = 0;
				largest_size = 0;
				for (i = 0; i < number_objects; i++) {
					if (blocks[i].type == PIXY_BLOCKTYPE_NORMAL &&
						blocks[i].width * blocks[i].height > largest_size &&
						blocks[i].width <= blocks[i].height*2) {
						largest_size = blocks[i].width * blocks[i].height;
						largest = i;
					}
				}
				try {
					pixy_mutex.lock();
					memcpy(&largest_block, &blocks[largest], sizeof(largest_block));
					largest_block.x -= (PIXY_MAX_X + 1) / 2;
					largest_block.y -= (PIXY_MAX_Y + 1) / 2;

					pixy_mutex.unlock();
				} catch (system_error& e) {
					cerr << "Pixy mutex: " << e.what() << endl;
				}
			}
		} else
			pixy_clear();
	}
}

void pixy_cam_get(pixy_block_t *output) {
	try {
		pixy_mutex.lock();
		memcpy(output, &largest_block, sizeof(largest_block));
		pixy_mutex.unlock();
	} catch (system_error& e) {
		cerr << "Pixy mutex: " << e.what() << endl;
	}
}
