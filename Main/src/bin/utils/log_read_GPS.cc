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
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <exception>
#include <stdexcept>

#include "GPS.hh"

using namespace std;

void parseJson(rapidjson::Document& document, char filename[]) {
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

	//double latitude, longitude, std_latitude, std_longitude;
	Trekking::GPS origin, current;
	cout << fixed << setprecision(9);
	rapidjson::Value::ConstMemberIterator it = document.MemberBegin();
	for (; it != document.MemberEnd(); ++it) {
		if (it->name == "latlon") {
			origin.latitude = it->value["latitude"].GetDouble();
			origin.longitude = it->value["longitude"].GetDouble();
			break;
		}
	}
	for (; it != document.MemberEnd(); ++it) {
		if (it->name == "latlon") {
			Trekking::GPS current, std_deviation;
			Eigen::Vector2d coordinate;
			current.latitude = it->value["latitude"].GetDouble();
			current.longitude = it->value["longitude"].GetDouble();
			current.to_2d(coordinate, origin);
			cout << 1000 * coordinate[0] << " " << 1000 * coordinate[1] << " ";

			// std_deviation.latitude = it->value["latitude std"].GetDouble();
			// std_deviation.longitude = it->value["longitude std"].GetDouble();
			// std_deviation.to_2d(coordinate, origin);
			// cout << 1000 * coordinate.x << " " << 1000 * coordinate.y << endl;
			cout << it->value["latitude std"].GetDouble() << endl;
		}
	}
}
