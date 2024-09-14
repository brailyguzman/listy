#include "../include/listy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/colors.h"
#include "../include/todo_io.h"

Todo create_todo(char *text) {
	int lines = count_lines();
	const char *path = get_todo_path();

	Todo newTodo = {
		.id = lines + 1,
		.text = text,
		.completed = 0,
		.created = time(NULL),
	};

	FILE *fptr;

	fptr = fopen(path, "a");
	fprintf(fptr, "%d__-%s__-%d__-%ld\n", newTodo.id, newTodo.text,
			newTodo.completed, newTodo.created);
	fclose(fptr);

	free((void *)path);

	return newTodo;
}

// TODO: Implement a better structure of the data printed.
void show_todo() {
	const char *path = get_todo_path();

	FILE *fptr;
	fptr = fopen(path, "r");
	char line[500];
	const char delimiter[] = "__-";

	while (fgets(line, sizeof(line), fptr)) {
		line[strcspn(line, "\n")] = '\0';
		char *part = strtok(line, delimiter);
		int count = 0;

		while (part != NULL) {
			switch (count) {
				case 0:
					printf("ID: %s | ", part);
					break;
				case 1:
					printf("Text: %s | ", part);
					break;
				case 2:
					if (strcmp(part, "0") == 0) {
						printf("Completed: %sFALSE%s | ", YEL, RESET);
					} else {
						printf("Completed: %sTRUE%s | ", GRN, RESET);
					}

					break;
				case 3:
					printf("Created: %s", part);
					break;
				default:
					break;
			}

			count++;
			part = strtok(NULL, delimiter);
		}

		printf("\n");
	}
	fclose(fptr);
	free((void *)path);
}