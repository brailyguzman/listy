#ifndef TODO_IO_H
#define TODO_IO_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "dirent.h"

char* get_path(char* file_path);
void clear_todos();

#endif