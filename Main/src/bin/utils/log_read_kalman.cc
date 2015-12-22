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

#include <cmath>
#include <iomanip>
#include <iostream>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include "TrekkingKF.hh"
#include "GPS.hh"

using namespace std;
using namespace Trekking;

static void parseJson(rapidjson::Document& document, char filename[]) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		perror("fopen");
		throw runtime_error("fopen");
	}

	char *file_buffer = new char[100 * 1024 * 1024];
	rapidjson::FileReadStream stream(file, file_buffer, sizeof(file_buffer));
	document.ParseStream(stream);

	fclose(file);
	delete[] file_buffer;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		cerr << "Uso: " << argv[0] << " arquivo" << endl;
		return -1;
	}

	rapidjson::Document document;
	parseJson(document, argv[1]);

	cout << fixed << setprecision(8);

	TrekkingKF filter;
	GPS origin;

	rapidjson::Value::ConstMemberIterator it = document.MemberBegin();
	double last_time;
	double angle;
	for (; it != document.MemberEnd(); ++it) {
		if (it->name == "latlon") {
			origin.latitude = it->value["latitude"].GetDouble();
			origin.longitude = it->value["longitude"].GetDouble();
			last_time = it->value["time"].GetDouble();
			break;
		}
		if (it->name == "bussola") {
			angle = it->value.GetDouble();
		}
	}

	DifferentialGPSMonitor differential_monitor(&origin);
	for (; it != document.MemberEnd(); ++it) {
		if (it->name == "latlon") {
			last_time = it->value["time"].GetDouble();
			differential_monitor.latitude = it->value["latitude"].GetDouble();
			differential_monitor.longitude = it->value["longitude"].GetDouble();
			differential_monitor.set_position();
			filter.update_position(differential_monitor.position[0] * 1000);
		}
		if (it->name == "bussola") {
			angle = it->value.GetDouble();
		}
		if (it->name == "aceleracao") {
			double current_time = it->value["time"].GetDouble();
			filter.update_accelerometer(it->value["x"].GetDouble() * cos(angle), current_time - last_time);
			last_time = current_time;
		}
	}
}
