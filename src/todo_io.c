#include "../include/todo_io.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void ensure_directory_exists(const char *path) {
	char *dir_name = strdup(path);

	if (dir_name == NULL) {
		perror("Error duplicating path");
		return;
	}

	char *last_slash = strrchr(dir_name, '/');

	if (last_slash == NULL) {
		perror("Error locating the last occurrence of the forward slash");
		return;
	}

	*last_slash = '\0';

	if (mkdir(dir_name, 0755) != 0) {
		perror("Error creating directory");
		return;
	}

	free(dir_name);
}

const char *get_todo_path() {
	const char *home = getenv("HOME");

	if (home == NULL) {
		perror("Error getting home directory");
		return NULL;
	}

	char *full_path = malloc(strlen(home) + strlen("/listy/todo.txt") + 1);

	if (full_path == NULL) {
		perror("Error allocating memory for the todo path");
		return NULL;
	}

	strcpy(full_path, home);
	strcat(full_path, "/listy/todo.txt");

	return full_path;
}

int count_lines() {
	const char *full_path = get_todo_path();

	FILE *fptr;
	fptr = fopen(full_path, "r");

	if (fptr == NULL) {
		ensure_directory_exists(full_path);
		fptr = fopen(full_path, "w");

		if (fptr == NULL) {
			perror("Error opening file\n");
			return -1;
		}
	}

	int ch;
	int lines = 0;

	while ((ch = getc(fptr)) != EOF) {
		if (ch == '\n') {
			lines++;
		}
	}

	fclose(fptr);
	free((void *)full_path);

	return lines;
}