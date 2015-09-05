#include <iostream>
#include <ctime>
#include <thread>
#include <mutex>
#include <signal.h>

#include <rapidjson/prettywriter.h>

#include "GPS.hh"
#include "udp_receiver.h"
#include "ports.h"
#include "compass.h"

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
            const struct timespec sleep_time = {.tv_sec = 1, .tv_nsec = 0};
            cerr << "blocking_update error" << endl;
            if (nanosleep(&sleep_time, NULL))
                perror("nanosleep");
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

    int udp_socket;
	int16_t data[3];
	if ((udp_socket = udp_receiver_init(UDP_HMC5883L, sizeof(data))) == -1)
		cerr << "hmc5883l_thread - udp_sender_init" << endl;
	else
        while (running) {
            switch (udp_receiver_recv(udp_socket, data, sizeof(data))) {
    			case sizeof(data): {
                    lock_guard<mutex> lock(json_output);
                    json.Key("bussola");
                    json.Double(compass_orientation(data[0], data[2]));
                }
    			break;

    			case -1:
    			perror("recvfrom");
    			break;

    			default:
    			cerr << "Unexpected message size" << endl;
    			break;
    		}
        }

    gps.join();
    json.EndObject();
    return 0;
}
