#ifndef LISTY_H
#define LISTY_H

typedef struct {
	char* text;
	int completed;	// 0: True, 1: False
} Todo;

void add(char* text);

#endif