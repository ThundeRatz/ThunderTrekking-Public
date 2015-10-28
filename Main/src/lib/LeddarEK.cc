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
#include <iostream>

#include "LeddarEK.hh"
#include "sleep.hh"

#define len(array)     ((&array)[1] - array)

static std::mutex leddarMtx;
static LdDetection measurement;
static unsigned int _nMeasures;

namespace Trekking {
	static unsigned char ReadData(void* aHandle, LeddarU32 aLevels) {
		LdDetection lDetections[50];

		int error;
		LtChar msg[256];

		unsigned int lCount = LeddarGetDetectionCount(aHandle);
		float min_distance = 999999.;

	    if (lCount > len(lDetections))
	        lCount = len(lDetections);

		leddarMtx.lock();
		_nMeasures = lCount;
		leddarMtx.unlock();

	    if ((error = LeddarGetDetections(aHandle, lDetections, len(lDetections))) != LD_SUCCESS) {
			LeddarGetErrorMessage(error, msg, len(msg));
			std::cerr << "Leddar get detetections error: " << msg << std::endl;
		}

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

		if (LeddarConnect(gHandle, "") != LD_SUCCESS)
			throw std::runtime_error("Leddar connection failed");

		if (LeddarStartDataTransfer(gHandle, LDDL_DETECTIONS) != LD_SUCCESS)
			throw std::runtime_error("Leddar start data transfer failed");

		if (LeddarAddCallback(gHandle, ReadData, gHandle) != LD_SUCCESS)
			throw std::runtime_error("Leddar add callback failed");

		sleep_ms(50);
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
		nMeasures = _nMeasures;
		leddarMtx.unlock();
	}

	void LeddarEK::restart() {
		LeddarStopDataTransfer(gHandle);
		LeddarRemoveCallback( gHandle, ReadData, gHandle );

		sleep_ms(200);

		if (LeddarStartDataTransfer(gHandle, LDDL_DETECTIONS) != LD_SUCCESS)
			throw std::runtime_error("Leddar start data transfer failed");

		if (LeddarAddCallback(gHandle, ReadData, gHandle) != LD_SUCCESS)
			throw std::runtime_error("Leddar add callback failed");
	}
}
