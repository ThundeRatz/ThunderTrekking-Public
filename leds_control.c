#define _GNU_SOURCE
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mod_i2c.h"
#include "leds.h"

#define len(array)	((&array)[1] - array)

#define OPTIONS		"r:g:b:m:t:h"

int main(int argc, char **argv) {
	int arg, index = 0, i;
	uint8_t r, g, b, m, t;
	static struct option long_options[] = {
		{"red",		required_argument, 0, 'r'},
		{"green",	required_argument, 0, 'g'},
		{"blue",	required_argument, 0, 'b'},
		{"mode",	required_argument, 0, 'm'},
		{"timestep",required_argument, 0, 't'},
		{"help",	no_argument, 0, 'h'},
		{0, 0, 0, 0}
	};
	static const char *help[] = {"Canal vermelho", "Canal verde", "Canal azul", "Modo de operação (MANUAL, BLINK, PULSE, PULSE3)", "Timestep", "Ajuda"};
	
	mod_i2c_create();
	
	for (;;) {
		arg = getopt_long(argc, argv, OPTIONS, long_options, &index);
		
		switch (arg) {
			case 'r':
			r = strtoul(optarg, NULL, 10);
			mod_i2c_write(REG_R, r);
			break;
			
			case 'g':
			g = strtoul(optarg, NULL, 10);
			mod_i2c_write(REG_G, g);
			break;
			
			case 'b':
			b = strtoul(optarg, NULL, 10);
			mod_i2c_write(REG_B, b);
			break;
			
			case 'm':
			if (!strcasecmp(optarg, "manual"))
				m = 0;
			else if (!strcasecmp(optarg, "blink"))
				m = 1;
			else if (!strcasecmp(optarg, "pulse"))
				m = 2;
			else if (!strcasecmp(optarg, "pulse3"))
				m = 3;
			else
				m = strtoul(optarg, NULL, 10);
			leds_mode(m);
			break;
			
			case 't':
			t = strtoul(optarg, NULL, 10);
			leds_timestep(t);
			break;
			
			case 'h':
			printf("Uso:\n");
			for (i = 0; i < len(help); i++)
				printf("\t-%c, --%s:\t%s\n", long_options[i].val, long_options[i].name, help[i]);
			
			case -1:
#warning SYNC
			return 0;
		}
	}
}
