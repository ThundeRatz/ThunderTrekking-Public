#define _GNU_SOURCE
#include <stdio.h>
#include <search.h>
#include <ctype.h>

static struct hsearch_data hash_table;

typedef struct entry_string entry_string_t;
struct entry_string {
	char *string;
	entry_string_t *next;
};

enum {
	CONFIG_DOUBLE;
	CONFIG_STRING;
}

typedef union {
	double Double;
	char *String;
} config_t;

typedef struct {
	int type;
	config_t data;
} config_data_t;

static entry_string_t *list = NULL, *list_end = NULL;

void config_file_load(char *name) {
	FILE *f;
	char line[300];
	
	f = fopen(name, "r");
	if (f == NULL) {
		perror("fopen");
		abort();
	}
	
	if (!hcreate_r(500, &hash_table)) {
		perror("hcreate_r");
		abort();
	}
	
	while (fgets(line, sizeof(line), f)) {
		config_data_t *new;
		ENTRY e;
		int i;
		
		if (strlen(line) == 299 && line[298] != '\n') {
			fprintf(stderr, "config_file_load: linha muito longa ignorada\n");
			continue;
		}
		
		for (i = 0; line[i]; i++) {
			if (!isspace(line[i]))
				break;
		}
		if (line[i] == '#')
			continue;
		
		e.key = &line[i];
		
		for (line[i]; i++) {
			if (isspace(line[i]))
				break;
		}
		
		line[i] = 0;
		
		for (line[i]; i++) {
			if (!isspace(line[i]))
				break;
		}
		
		if (line[i]) {
			new = malloc(sizeof(config_data_t));
			if (new == NULL) {
				perror("malloc");
				abort();
			}
			new->type = (line[i] == '"') ? CONFIG_STRING : CONFIG_DOUBLE;
			e.data = new;
			if (new->type == CONFIG_STRING) {
				new->data.String = &line[i];
				for (line[i]; i++) {
					if (isspace(line[i]))
						break;
				}
				line[i] = 0;
			} else {
				new->data.Double = strtod(&line[i], NULL);
			}
		} else {
			e.data = NULL;
		}
		
		hsearch_r(e, ENTER, &retval, &hash_table);
	}
	
	if (!feof(f))
		perror("fgets");
		abort();
	}
	
	fclose(f);
}

config_data_t *config_file_get(char *name) {
	ENTRY e, *ret;
	e.key = name;
	if (hsearch_r(e, FIND, &ret, &hash_table)) {
		return ret->data;
	}
	return NULL;
}

void config_file_set(char *name, config_data_t *c) {
	ENTRY e;
	config_data_t *new;
	new = malloc(sizeof(config_data_t));
	if (new == NULL) {
		perror("malloc");
		abort();
	}
	memcpy(new, c, sizeof(config_data_t));
	e.key = name;
	e.data = new;
	if (!hsearch_r(e, ENTER, &retval, &hash_table)) {
		perror("hsearch_r");
		abort();
	}
}
