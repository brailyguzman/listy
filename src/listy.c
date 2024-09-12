#include "../include/listy.h"

#include "../include/todo_io.h"

Todo create_todo(char* text) {
	int lines = count_lines();
	const char* path = get_todo_path();

	Todo newTodo = {
		.id = lines + 1,
		.text = text,
		.completed = 0,
		.created = time(NULL),
	};

	FILE* fptr;

	fptr = fopen(path, "a");
	fprintf(fptr, "%d__-%s__-%d__-%ld\n", newTodo.id, newTodo.text,
			newTodo.completed, newTodo.created);
	fclose(fptr);

	return newTodo;
}