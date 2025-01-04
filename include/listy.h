#ifndef LISTY_H
#define LISTY_H

#include <colors.h>
#include <todo_io.h>

#define MAX_TASK_LENGTH 37
#define LINE_LENGTH 256

#define TASK_SIZE 128
#define ID_SIZE 16
#define STATUS_SIZE 32

#define TEMP_FILE_NAME "/temp.txt"
#define TODOS_FILE_NAME "/todos.txt"

void view_todos();
void clear_todos();
int new_id(char *path);
int add_todo(char *text);
int edit_todo(int id, int remove);

#endif
