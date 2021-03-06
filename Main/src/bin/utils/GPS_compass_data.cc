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
#include <ctime>
#include <thread>
#include <mutex>

#include <signal.h>
#include <rapidjson/prettywriter.h>

#include "BNO055.hh"
#include "GPS.hh"
#include "UDP.hh"
#include "sleep.hh"
#include "errno_string.hh"

#include "ports.h"
#include "Compass.hh"

using namespace std;
using namespace Trekking;

static int running = 1;

static void sig_handler(int signo) {
    if (signo == SIGINT)
        running = 0;
}

// rapidjson usa um tipo estranho de stream, então temos uma classe de interface
// para cout bem simples
class JSON_stream_to_cout_interface {
public:
    void Put(const char c) {cout << c;}
    void Flush() {}
    void Clear() {}
    void ShrinkToFit() {}
};

// Usar DOM é uma boa opção com uma interface mais limpa:
/*
rapidjson::Document json;
rapidjson::Document::AllocatorType& json_allocator = json.GetAllocator();
rapidjson::Value latlon_data(rapidjson::kObjectType);
latlon_data.AddMember("latitude", gps.gpsd_data->fix.latitude, json_allocator)
           .AddMember("latitude std", gps.gpsd_data->fix.epy, json_allocator)
           .AddMember("longitude", gps.gpsd_data->fix.longitude, json_allocator)
           .AddMember("longitude std", gps.gpsd_data->fix.epx, json_allocator);
json.PushBack("latlon", latlon_data, json_allocator);
*/
// Mas usaremos a classe Writer diretamente com um mutex, pela vantagem de usar
// um stream e gerar a saída conforme enviamos os dados e não tudo no fim do
// programa.

JSON_stream_to_cout_interface cout_interface;
rapidjson::PrettyWriter<JSON_stream_to_cout_interface> json(cout_interface);
mutex json_output;

void gps_thread() {
    GPSMonitor gps;

#if GPSD_API_MAJOR_VERSION != 6
#warning GPSd API 6 required
    cout << "GPSd API 6 required" << endl;
#else
    while (running) {
        if (gps.update()) {
            if (gps.gpsd_data != NULL) {
                if (gps.gpsd_data->set & LATLON_SET) {
                    lock_guard<mutex> lock(json_output);
                    json.Key("latlon");
                    json.StartObject();
                    json.Key("latitude");
                    json.Double(gps.gpsd_data->fix.latitude);
                    json.Key("latitude std");
                    json.Double(gps.gpsd_data->fix.epy);
                    json.Key("longitude");
                    json.Double(gps.gpsd_data->fix.longitude);
                    json.Key("longitude std");
                    json.Double(gps.gpsd_data->fix.epx);
                    json.Key("time");
                    struct timespec event_time;
                    clock_gettime(event_time);
                    json.Uint64(event_time.tv_sec * 1000 + event_time.tv_nsec / 1000000);
                    json.EndObject();
                }

                if ((gps.gpsd_data->set & (SPEED_SET | SPEEDERR_SET)) == (SPEED_SET | SPEEDERR_SET)) {
                    lock_guard<mutex> lock(json_output);
                    json.Key("speed");
                    json.StartObject();
                    json.Key("value");
                    json.Double(gps.gpsd_data->fix.speed);
                    json.Key("std");
                    json.Double(gps.gpsd_data->fix.eps);
                    json.Key("time");
                    struct timespec event_time;
                    clock_gettime(event_time);
                    json.Uint64(event_time.tv_sec * 1000 + event_time.tv_nsec / 1000000);
                    json.EndObject();
                } else if (gps.gpsd_data->set & (SPEED_SET | SPEEDERR_SET)) {
                    // Para testes iniciais do Kalman, vamos manter incertezas e
                    // medidas juntas. Se usarmos um GPS com protocolo estranho
                    // que as forcene separadas, dar aviso
                    cerr << "SPEED_SET e SPEEDERR_SET fornecidos separados" << endl
                         << "Programa espera SPEED_SET e SPEEDERR_SET do GPS juntos" << endl;
                }

                gps.gpsd_data->set = 0;
            }
        } else {
            unsigned int sleep_time = 100;
            cerr << "blocking_update error" << endl;
            sleep_ms(sleep_time);
        }
    }
#endif
}

int main() {
    struct sigaction action;
	memset((char *) &action, 0, sizeof(action));
	action.sa_handler = sig_handler;
	if (sigaction(SIGINT, &action, NULL))
        perror("sigaction");

    json.SetIndent('\t', 1);
    json.StartObject();

    thread gps(gps_thread);

	int16_t data[3];

	try {
        Trekking::BNO055 bno055;
        while (running) {
            double angle = bno055.read_angle();
            {
                lock_guard<mutex> lock(json_output);
                json.Key("bussola");
                json.StartObject();
                json.Key("angle");
                json.Double(angle);
                json.Key("time");
                struct timespec event_time;
                clock_gettime(event_time);
                json.Uint64(event_time.tv_sec * 1000 + event_time.tv_nsec / 1000000);
                json.EndObject();
            }

            Eigen::Vector2d acceleration;
            bno055.linear_acceleration(acceleration);
            {
                lock_guard<mutex> lock(json_output);
                json.Key("aceleracao");
                json.StartObject();
                json.Key("x");
                json.Double(acceleration[0]);
                json.Key("y");
                json.Double(acceleration[1]);
                json.Key("time");
                struct timespec event_time;
                clock_gettime(event_time);
                json.Uint64(event_time.tv_sec * 1000 + event_time.tv_nsec / 1000000);
                json.EndObject();
            }
        }
    } catch (runtime_error& e) {
        cerr << e.what() << endl;
    }

    gps.join();
    json.EndObject();
    return 0;
}
