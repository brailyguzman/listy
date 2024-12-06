#ifndef LISTY_H
#define LISTY_H

#include <colors.h>
#include <errno.h>
#include <todo_io.h>

#define MAX_TASK_LENGTH 37
#define TASK_SIZE 128

void view_todos();
int new_id(char* path);
int add(char* text);

#endif