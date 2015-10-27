/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <stdexcept>
#include <cstring>

#include "Pixy.hh"

#define len(array)	((&array)[1] - array)

namespace Trekking {
	PixyCam::PixyCam() {
		int error_code;
		if ((error_code = pixy_init())) {
			pixy_error(error_code);
			throw std::runtime_error("pixy_init");
		x = y = 0;
		}

#ifdef DEBUG
		uint16_t major, minor, build;
		if ((error_code = pixy_get_firmware_version(&major, &minor, &build)))
			pixy_error(error_code);
		else
			std::cout << "Pixy firmware: " << major << '.' << minor << '.' << build << std::endl;
#endif
	}

	void PixyCam::clear() {
		memset(&block, 0, sizeof(block));
		x = y = 0;
	}

	void PixyCam::update() {
		int number_objects, i, largest, largest_size;

		if (pixy_blocks_are_new()) {
			number_objects = pixy_get_blocks(len(blocks), blocks);

			if (!number_objects)
				clear();
			else if (number_objects < 0) {
				pixy_error(number_objects);
			} else {
#ifdef PIXY_DEBUG
				std::cout << "PIXY:  "<< number_objects << "blocks received\n";
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
				memcpy(&block, &blocks[largest], sizeof(block));
				x = block.x - (PIXY_MAX_X + 1) / 2;
				y = block.y - (PIXY_MAX_Y + 1) / 2;
			}
		} else
			clear();
	}
}
