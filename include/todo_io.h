#include <stdio.h>

#ifndef TODO_IO
#define TODO_IO

const char* TODO_PATH;

void ensure_directory_exists(const char*);
const char* get_todo_path();
int count_lines();

#endif