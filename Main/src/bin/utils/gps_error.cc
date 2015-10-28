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

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctime>

#include "GPS.hh"
#include "sleep.hh	"

#include "compass.h"

#define MS	1000000
#define len(array)	((&array)[1] - array)

using namespace std;

static const struct timespec gps_wait = {.tv_sec = 1, .tv_nsec = 0 * MS};

Trekking::GPS testes[] = {
	Trekking::GPS(-23.55392375, -46.72891395),  //Mesa elétrica
	Trekking::GPS(-23.55387477, -46.72899905),  //Interruptor
	Trekking::GPS(-23.55344336, -46.72915281),  //Hidrante
	//{.latitude = -23.64701287, .longitude = -46.57263634},
};

int main() {
	cout << fixed << setprecision(10);

	double angle, dist;

	// Media das medidas do GPS na posicao atual
	Trekking::GPSStats stats;
	Trekking::GPSMonitor position(Trekking::GPS(0., 0.));
	for (int n = 1; n <= 15; n++) {
		while (!position.blocking_update()) ;
		cout << "Posicao atual: " << position.latitude << " " << position.longitude << "\n";
		stats.sample(position);
		Trekking::sleep_ms(1000);
	}
	cout << "Posicao atual media: " << stats.latitude_stats().mean() << " " << stats.longitude_stats().mean() << "\n";

	return 0;
}
