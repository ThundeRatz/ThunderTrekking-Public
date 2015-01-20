#ifndef __GPS_H__
#define __GPS_H__

typedef struct {
	double latitude, longitude;
} gps_coord_t;

double gps_distance(const gps_coord_t *from, const gps_coord_t *to);
double azimuth(const gps_coord_t *from, const gps_coord_t *to);

#endif
