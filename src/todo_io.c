#include "../include/todo_io.h"

const char* TODO_PATH = "~/listy/todo.txt";

int count_lines() {
	FILE* fptr;
	fptr = fopen(TODO_PATH, "r+");

	if (fptr == NULL) {
		fptr = fopen(TODO_PATH, "w+");

		if (fptr == NULL) {
			perror("Error opening file");
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

	return lines;
}