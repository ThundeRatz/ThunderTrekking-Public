#ifndef __COMMAND_H__
#define __COMMAND_H__

typedef struct {
	// coordenadas
	// distância do alvo
	// sonares/kinect
} gps_command_t;

typedef struct {
	
} vision_command_t;

enum {
	COMMAND_GPS,
	COMMAND_VISION
};

typedef enum {
	int type;
	gps_command_t gps;
	vision_command_t vision;
} command_e;

command_e commands[] = {
	{
		.type = COMMAND_GPS,
	},
	
	{
		.type = COMMAND_VISION,
	}
};

#endif
