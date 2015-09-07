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
#include <cstdio>
#include <ctime>

#include "GPS.hh"

using namespace std;

const char *format_string(const char *string) {
    static const char string_null[] = "NULL";
    static const char string_empty[] = "EMPTY";
    if (string == NULL)
        return string_null;
    if (*string == 0)
        return string_empty;
    return string;
}

int main() {
    Trekking::GPSMonitor gps(Trekking::GPS(0., 0.));

    for (;;) {
        if (gps.blocking_update()) {
            if (gps.gpsd_data == NULL)
                cout << "NULL gpsd_data" << endl;
            else {
#if GPSD_API_MAJOR_VERSION != 6
#warning GPSd API 6 required, not compiling
                cout << "GPSd API 6 required" << endl;
#else
                string set_fields[] = {
                    "?",
                    "ONLINE_SET",
                    "TIME_SET",
                    "TIMERR_SET",
                    "LATLON_SET",
                    "ALTITUDE_SET",
                    "SPEED_SET",
                    "TRACK_SET",
                    "CLIMB_SET",
                    "STATUS_SET",
                    "MODE_SET",
                    "DOP_SET",
                    "HERR_SET",
                    "VERR_SET",
                    "ATTITUDE_SET",
                    "SATELLITE_SET",
                    "SPEEDERR_SET",
                    "TRACKERR_SET",
                    "CLIMBERR_SET",
                    "DEVICE_SET",
                    "DEVICELIST_SET",
                    "DEVICEID_SET",
                    "RTCM2_SET",
                    "RTCM3_SET",
                    "AIS_SET",
                    "PACKET_SET",
                    "SUBFRAME_SET",
                    "GST_SET",
                    "VERSION_SET",
                    "POLICY_SET",
                    "LOGMESSAGE_SET",
                    "ERROR_SET",
                    "TOFF_SET",
                    "PPS_SET",
                    "SET_HIGH_BIT",
                };

                cout << "devices: " << gps.gpsd_data->devices.ndevices << "\n";
                cout << "set - " << gps.gpsd_data->set << "\n";
                for (unsigned int i = 0; i < sizeof(set_fields) / sizeof(*set_fields); i++)
                    if (gps.gpsd_data->set & (1 << i))
                        cout << "\t" << set_fields[i] << "\n";

                if (gps.gpsd_data->online)
                    cout << "Last online " << gps.gpsd_data->online << endl;
                else
                    cout << "Offline\n";

                if (gps.gpsd_data->status == STATUS_FIX) {
                    cout << "Fix - " << gps.gpsd_data->satellites_used << " satellites used\n";
                    cout << "visible: " << gps.gpsd_data->satellites_visible << "\n";
                    cout << "Skyview time: " << gps.gpsd_data->skyview_time << endl;
#ifdef DEBUG
                    // Esses dados podem ser usados para ver qualidade de cada
                    // satélite, mas são deixados de lado por padrão por serem
                    // bem barulhentos
                    for (unsigned int i = 0; i < sizeof(gps.gpsd_data->skyview) / sizeof(*gps.gpsd_data->skyview); i++)
                        if (gps.gpsd_data->skyview[i].ss != 0. || gps.gpsd_data->skyview[i].used ||
                            gps.gpsd_data->skyview[i].PRN != 0 || gps.gpsd_data->skyview[i].elevation != 0 ||
                            gps.gpsd_data->skyview[i].azimuth != 0) {

                            cout << "Sat " << i << "\n\t"
                                << "signal-to-noise ratio (dB): " << gps.gpsd_data->skyview[i].ss << "\n\t"
                                << "elevation: " << gps.gpsd_data->skyview[i].elevation << "\n\t"
                                << "azimuth: " << gps.gpsd_data->skyview[i].azimuth << "\n\t";
                            if (gps.gpsd_data->skyview[i].used)
                                cout << "PRN: " << gps.gpsd_data->skyview[i].PRN << "\n";
                            else
                                cout << "No PRN" << "\n";
                        }
#endif

                    cout << "spherical position error (95% confidence): " << gps.gpsd_data->epe << "\n";
                    cout << "fix" << "\n\t"
                        << "time (±" << gps.gpsd_data->fix.ept << ") " << gps.gpsd_data->fix.time << "\n\t"
                        << "mode: " << gps.gpsd_data->fix.mode << " {MODE_NOT_SEEN, MODE_NO_FIX, MODE_2D, MODE_3D}" << "\n\t"
                        << "latitude: " << gps.gpsd_data->fix.latitude << " (±" << gps.gpsd_data->fix.epy << ")\n\t"
                        << "longitude: " << gps.gpsd_data->fix.longitude << " (±" << gps.gpsd_data->fix.epx << ")\n\t"
                        << "altitude: " << gps.gpsd_data->fix.altitude << " (±" << gps.gpsd_data->fix.epv << ")\n\t"
                        << "track: " << gps.gpsd_data->fix.track << " (±" << gps.gpsd_data->fix.epd << ")\n\t"
                        << "speed: " << gps.gpsd_data->fix.speed << " (±" << gps.gpsd_data->fix.eps << ")\n\t"
                        << "climb: " << gps.gpsd_data->fix.climb << " (±" << gps.gpsd_data->fix.epc << ")\n";
                } else
                    cout << "No fix\n";

                if (gps.gpsd_data->set & DEVICE_SET)
                    cout << "dev" << "\n\t"
        		        << "path: " << format_string(gps.gpsd_data->dev.path) << "\n\t"
                        << "flags: " << gps.gpsd_data->dev.flags << " (1 = SEEN_GPS, 2 = SEEN_RTCM2, 4 = SEEN_RTCM2, 8 = SEEN_AIS)" << "\n\t"
                        << "driver: " << format_string(gps.gpsd_data->dev.driver) << "\n\t"
                        << "subtype: " << format_string(gps.gpsd_data->dev.subtype) << "\n\t"
                        << "activated: " << gps.gpsd_data->dev.activated << "\n\t"
                        << "baud: " << gps.gpsd_data->dev.baudrate << "\n\t"
                        << "stopbits: " << gps.gpsd_data->dev.stopbits << "\n\t"
                        << "parity: " << gps.gpsd_data->dev.parity << "\n\t"
                        << "cicle/mincicle: " << gps.gpsd_data->dev.cycle << "/" << gps.gpsd_data->dev.mincycle << "\n\t"
                        << "native: " << gps.gpsd_data->dev.driver_mode << "\n";

                if (gps.gpsd_data->set & DOP_SET)
                    cout << "DOP" << "\n\t"
                        << "x: " << gps.gpsd_data->dop.xdop << "\t"
                        << "y: " << gps.gpsd_data->dop.ydop << "\t"
                        << "p: " << gps.gpsd_data->dop.pdop << "\t"
                        << "h: " << gps.gpsd_data->dop.hdop << "\t"
                        << "v: " << gps.gpsd_data->dop.vdop << "\t"
                        << "t: " << gps.gpsd_data->dop.tdop << "\t"
                        << "g: " << gps.gpsd_data->dop.gdop << "\n";

                if (gps.gpsd_data->set & POLICY_SET)
                    cout << "Policy" << "\n\t"
                        << "watcher: " << gps.gpsd_data->policy.watcher << "\n\t"
                        << "json: " << gps.gpsd_data->policy.json << "\n\t"
                        << "nmea: " << gps.gpsd_data->policy.nmea << "\n\t"
                        << "raw: " << gps.gpsd_data->policy.raw << "\n\t"
                        << "scaled: " << gps.gpsd_data->policy.scaled << "\n\t"
                        << "timing: " << gps.gpsd_data->policy.timing << "\n\t"
                        << "split24: " << gps.gpsd_data->policy.split24 << "\n";
                if (gps.gpsd_data->set & GST_SET)
                    cout << "Pseudorange deviations" << "\n\t"
                        << "time: " << ctime((time_t *) &gps.gpsd_data->gst.utctime) << "\t"
                        << "rms: " << gps.gpsd_data->gst.rms_deviation << "\n\t"
                        << "s: major " << gps.gpsd_data->gst.smajor_deviation
                        << ", minor " << gps.gpsd_data->gst.sminor_deviation
                        << ", orientation " << gps.gpsd_data->gst.smajor_orientation << "\n\t"
                        << "latitude: " << gps.gpsd_data->gst.lat_err_deviation << "\n\t"
                        << "longitude: " << gps.gpsd_data->gst.lon_err_deviation << "\n\t"
                        << "altitude: " << gps.gpsd_data->gst.alt_err_deviation << "\n";
                cout << "\n\n";

                gps.gpsd_data->set = 0;
            }
#endif
        } else {
            const struct timespec sleep_time = {.tv_sec = 1, .tv_nsec = 0};
            cout << "blocking_update error" << endl;
            if (nanosleep(&sleep_time, NULL))
                perror("nanosleep");
        }
    }
}
