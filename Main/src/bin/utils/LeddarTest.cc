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
#include <mutex>

#include "LeddarEK.hh"
#include "sleep.hh"

#define len(array)     ((&array)[1] - array)

using namespace std;
using namespace Trekking;

int main() {
	LeddarEK leddar;
	int cont = 0;
	float ant = 0.;

	for(;;) {
		leddar.update();

		if (ant == leddar.measure.mDistance) cont++;
		else cont = 0;

		if (cont >= 5) {
			leddar.restart();
			cont = 0;
			ant = 0.;
		}

		ant = leddar.measure.mDistance;

		cout << "Minimum Measurement: \n"
			<< "\t" << leddar.measure.mSegment << " | " << leddar.measure.mDistance << endl
			<< "Number of measures: " << leddar.nMeasures << endl;

		if (leddar.measure.mSegment < 6 && leddar.measure.mDistance < 0.3)
			cout << "Esquerda\n";
		else if (leddar.measure.mSegment > 8 && leddar.measure.mDistance < 0.3)
			cout << "Direita\n";
		else
			cout << "Reto\n";

		sleep_ms(300);
	}
}
