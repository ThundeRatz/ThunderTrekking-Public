#ifndef __CONFIG_H__
#define __CONFIG_H__

enum {
	CONFIG_NULL;
	CONFIG_INT;
	CONFIG_DOUBLE;
	CONFIG_STRING;
}

typedef union {
	int Int;
	double Double;
	char *String;
} config_t;

typedef struct {
	int type;
	config_t data;
} config_data_t;

void config_file_load(char *name);
config_data_t *config_file_get(char *name);
void config_file_set(char *name, config_data_t *c);

#endif
