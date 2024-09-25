#include <stdio.h>

#ifndef TODO_IO
#define TODO_IO

void ensure_directory_exists(const char*);
const char* get_todo_path();
const char* get_temp_path();
int count_lines();

#endif