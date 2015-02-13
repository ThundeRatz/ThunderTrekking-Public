#ifndef __GPS_UDP_H__
#define __GPS_UDP_H__

int gps_udp_init();
int gps_udp_recv(gps_coord_t *data);
int gps_udp_has_data();

#endif
