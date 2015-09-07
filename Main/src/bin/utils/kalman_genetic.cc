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

#include <ga/GASimpleGA.h>
#include <ga/GABin2DecGenome.h>
#include <iostream>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include "GPS.hh"
#include "TrekkingKF.hh"

#define len(array)	(sizeof(array) / sizeof(array[0]))

using namespace std;
using namespace Trekking;

float genetic_objective(GAGenome &);

static rapidjson::Document document;

int main() {
	GABin2DecPhenotype map;

	FILE* file = fopen("kalman_input.dat", "r");
	if (file == NULL) {
		perror("fopen");
		throw runtime_error("fopen");
	}

	char *file_buffer = new char[100 * 1024 * 1024];
	rapidjson::FileReadStream stream(file, file_buffer, sizeof(file_buffer));
	document.ParseStream(stream);
	fclose(file);
	delete[] file_buffer;

	float min[] = {0, 1,   1,   1,   100,  0.5, 0.5};
	float max[] = {1, 100, 100, 100, 1000, 100, 7};
	for (unsigned int i = 0; i < len(min); i++)
		map.add(32, min[i], max[i]);

	float target[len(min)];
	for (unsigned int i = 0; i < len(min); i++)
		target[i] = GARandomFloat(min[i], max[i]);

	GABin2DecGenome genome(map, genetic_objective, (void *) target);
	GASimpleGA genetic(genome);
	genetic.populationSize(100);
	genetic.nConvergence(50);
	genetic.pConvergence(.999);
	genetic.pMutation(0.01);
	genetic.pCrossover(0.6);
	genetic.scoreFilename("kalman_genetic.dat");
	genetic.flushFrequency(50);
	genetic.evolve();

	return 0;
}

float genetic_objective(GAGenome& raw_genome) {
	TrekkingKF filtro;
	GABin2DecGenome& genome = (GABin2DecGenome&) raw_genome;

	//double latitude, longitude, std_latitude, std_longitude;
	Trekking::GPS origin, current;
	for (rapidjson::Value::ConstMemberIterator it = document.MemberBegin();
		it != document.MemberEnd(); ++it) {
		if (it->name == "bumper") {

			break;
		}
	}
	return 0.;
}
