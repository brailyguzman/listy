#include <time.h>

#ifndef LISTY_H
#define LISTY_H

typedef struct {
	int id;
	char* text;
	int completed;
	time_t created;
} Todo;

Todo create_todo(char*);

#endif