#include "../include/listy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void show_todo() {
	const char *path = get_todo_path();
	FILE *fptr = fopen(path, "r");
	if (fptr == NULL) {
		perror("Error opening file");
		return;
	}

	char line[500];
	const char delimiter[] = "__-";
	printf("%-5s %-30s %-10s %-20s\n", "ID", "Text", "Completed", "Created");
	printf("------------------------------------------------------------\n");

	while (fgets(line, sizeof(line), fptr)) {
		line[strcspn(line, "\n")] = '\0';
		char *part = strtok(line, delimiter);

		Todo todo;
		int count = 0;

		while (part != NULL) {
			switch (count) {
				case 0:
					todo.id = atoi(part);
					break;
				case 1:
					todo.text = part;
					break;
				case 2:
					todo.completed = atoi(part);
					break;
				case 3:
					todo.created = atol(part);
					break;
				default:
					break;
			}

			count++;
			part = strtok(NULL, delimiter);
		}

		char formatted_time[20];
		strftime(formatted_time, sizeof(formatted_time), "%Y-%m-%d %H:%M:%S",
				 localtime(&todo.created));

		printf("%-5d %-30s %-10s %-20s\n", todo.id, todo.text,
			   todo.completed ? "TRUE" : "FALSE", formatted_time);
	}

	fclose(fptr);
	free((void *)path);
}