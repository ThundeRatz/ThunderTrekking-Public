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

#include "LeddarEK.hh"
#include "sleep.hh"

#define len(array)     ((&array)[1] - array)

static std::mutex leddarMtx;
static LdDetection measurement;

namespace Trekking {
	unsigned char ReadData(void* aHandle, LeddarU32 aLevels) {
		LdDetection lDetections[50];
	    unsigned int lCount = LeddarGetDetectionCount(aHandle);
		float min_distance = 999999.;

	    if (lCount > len(lDetections))
	        lCount = len(lDetections);

	    LeddarGetDetections(aHandle, lDetections, len(lDetections));

	    for (unsigned int i = 0; (i < lCount) && (i < 16); ++i) {
	        if (lDetections[i].mDistance < min_distance) {
				min_distance = lDetections[i].mDistance;
				leddarMtx.lock();
				measurement = lDetections[i];
				leddarMtx.unlock();
			}
		}
	    return 1;
	}

	LeddarEK::LeddarEK() {
		gHandle = LeddarCreate();

		if(LeddarConnect(gHandle, "") != LD_SUCCESS)
			throw std::runtime_error("Leddar connection failed");

		LeddarStartDataTransfer(gHandle, LDDL_DETECTIONS);
		LeddarAddCallback(gHandle, ReadData, gHandle);
		sleep_ms(100);
	}

	LeddarEK::~LeddarEK() {
		LeddarStopDataTransfer(gHandle);
		LeddarRemoveCallback( gHandle, ReadData, gHandle );

		LeddarDisconnect(gHandle);
		LeddarDestroy(gHandle);
	}

	void LeddarEK::update() {
		leddarMtx.lock();
		measure = measurement;
		leddarMtx.unlock();
	}
}
