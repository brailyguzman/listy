#include <listy.h>

void view_todos() {
	// Get the todos.txt path - (allocated memory needs to be released).
	char* path = get_path("/todos.txt");

	// Check if the path is NULL
	if (path == NULL) {
		fprintf(stderr, "Error: Failed to get file path.\n");
		return;
	}

	// Open the file from the path we got - (stream needs to be closed).
	FILE* fptr = fopen(path, "r");

	// Check if we were able to open the file.
	if (fptr == NULL) {
		fptr = fopen(path, "w");

		if (fptr == NULL) {
			fprintf(stderr, "Error: Unable to open todos file (%s).\n", path);
			fclose(fptr);
			free(path);
			return;
		}

		printf("No todos to show!\n");
		fclose(fptr);
		free(path);

		return;
	}

	// Get the first character in the file.
	int ch = fgetc(fptr);

	// Check if the character is equivalent to the End of Line (EOF) character.
	// If so, this means the file is empty, and there are no todos to show.
	if (ch == EOF) {
		fclose(fptr);
		free(path);
		printf("No todos to show!\n");

		return;
	} else {
		// Push the character back to the stream.
		ungetc(ch, fptr);
	}

	// Create line variable with a size of LINE_LENGTH.
	char line[LINE_LENGTH];
	// Store the delimiter which will divide id, text, and status.
	char* delimiter = "|";

	// Print header with appropriate widths
	printf("\n%s%-5s %-40s %-20s%s\n", GRN, "ID", "Task", "Status", RESET);
	printf("%s%-5s %-40s %-20s%s\n", GRN, "-----",
		   "----------------------------------------", "-------------", RESET);

	int completed = 0;
	int incomplete = 0;

	// Get every line on the file.
	while (fgets(line, sizeof(line), fptr)) {
		// Check if any of the lines are invalid, and skip them.
		if (line[0] == '\0' || line[0] == '\n') {
			continue;
		}

		// Store a count representing which token we are working with.
		int count = 0;

		// Get the first token of the line. The initial will be the ID.
		char* token = strtok(line, delimiter);

		// Create variables to store the tokens we gather.
		char id[ID_SIZE] = "";
		char task[TASK_SIZE] = "";
		char status[STATUS_SIZE] = "";
		int is_completed = 0;

		while (token != NULL) {
			switch (count) {
				// ID
				case 0:
					strncpy(id, token, sizeof(id) - 1);
					break;
				// TASK
				case 1:
					strncpy(task, token, sizeof(task) - 1);
					break;
				// Completion
				case 2:
					// Store it as a human readable format.
					if (atoi(token) == 1) {
						snprintf(status, sizeof(status), "✅ Completed");
						is_completed = 1;
						completed++;
					} else {
						snprintf(status, sizeof(status), "❌ Incomplete");
						incomplete++;
					}
					break;
				default:
					break;
			}

			// Increase the count to go to the next token.
			count++;
			token = strtok(NULL, delimiter);
		}

		// Truncate the text to be displayed properly with the header.
		if (strlen(task) > MAX_TASK_LENGTH) {
			task[MAX_TASK_LENGTH] = '\0';
			strcat(task, "...");
		}

		if (is_completed) {
			printf("%s%-5s %-40s %-20s%s\n", GRN, id, task, status, RESET);
		} else {
			printf("%s%-5s %-40s %-20s%s\n", YEL, id, task, status, RESET);
		}

		// Display the Todo
	}

	int total = completed + incomplete;

	printf("\n%sTotal: %d%s | %sCompleted: %d%s | %sIncomplete: %d%s\n\n", YEL,
		   total, RESET, GRN, completed, RESET, MAG, incomplete, RESET);

	// Close the stream and release the memory for the path.
	fclose(fptr);
	free(path);
}

int get_new_id(char* path) {
	FILE* fptr = fopen(path, "r");

	if (fptr == NULL) {
		fprintf(stderr, "Error: Unable to open the todos file (%s): %s\n", path,
				strerror(errno));
		free(path);

		return -1;
	}

	char line[LINE_LENGTH];
	int max_id = 0;

	while (fgets(line, sizeof(line), fptr)) {
		if (line[0] == '\0' || line[0] == '\n') {
			continue;
		}

		char* id_str = strtok(line, "|");

		if (id_str != NULL) {
			int id = atoi(id_str);

			if (id > max_id) {
				max_id = id;
			}
		}
	}

	fclose(fptr);
	return max_id + 1;
}

int add_todo(char* text) {
	char* path = get_path("/todos.txt");

	if (path == NULL) {
		return -1;
	}

	FILE* fptr = fopen(path, "a");

	if (fptr == NULL) {
		fprintf(stderr, "Error: Failed to open the todos file.\n");
		free(path);
		return -1;
	}

	int todo_id = get_new_id(path);

	if (todo_id == -1) {
		return -1;
	}

	char line[LINE_LENGTH];

	text[strcspn(text, "\n")] = '\0';

	snprintf(line, sizeof(line), "%d|%s|%d\n", todo_id, text, 0);
	fputs(line, fptr);

	free(path);
	fclose(fptr);

	view_todos();

	return 0;
}

int edit_todo(int id, int delete) {
	char* todos_path = get_path(TODOS_FILE_NAME);
	char* temp_path = get_path(TEMP_FILE_NAME);

	if (temp_path == NULL || todos_path == NULL) {
		free(todos_path);
		free(temp_path);

		return -1;
	}

	FILE* todos_fptr = fopen(todos_path, "r");
	FILE* temp_fptr = fopen(temp_path, "w");

	if (todos_fptr == NULL) {
		fprintf(stderr, "Error: Unable to open the todos file (%s): %s\n",
				todos_path, strerror(errno));

		free(todos_path);
		free(temp_path);

		return -1;
	} else if (temp_fptr == NULL) {
		fprintf(stderr, "Error: Unable to open the temp file (%s): %s\n",
				temp_path, strerror(errno));
		fclose(todos_fptr);
		free(todos_path);
		free(temp_path);

		return -1;
	}

	char line[LINE_LENGTH];
	int found = 0;

	while (fgets(line, sizeof(line), todos_fptr)) {
		if (line[0] == '\0' || line[0] == '\n') {
			continue;
		}

		char* id_str = strtok(line, "|");
		char* line_task = strtok(NULL, "|");
		char* status_str = strtok(NULL, "|");

		if (id_str == NULL || line_task == NULL || status_str == NULL) {
			continue;
		}

		if (delete == 1 && atoi(id_str) == id) {
			found = 1;
		} else if (atoi(id_str) == id) {
			found = 1;
			int newStatus = 0;

			if (atoi(status_str) == 0) {
				newStatus = 1;
			}

			fprintf(temp_fptr, "%s|%s|%d\n", id_str, line_task, newStatus);
		} else {
			fprintf(temp_fptr, "%s|%s|%s\n", id_str, line_task, status_str);
		}
	}

	fclose(todos_fptr);
	fclose(temp_fptr);

	if (!found) {
		fprintf(stderr, "Error: Task with ID %d not found.\n", id);

		remove(temp_path);

		free(todos_path);
		free(temp_path);

		return -1;
	}

	if (remove(todos_path) != 0) {
		fprintf(stderr,
				"Error: Unable to remove original todos file (%s): %s\n",
				todos_path, strerror(errno));

		free(todos_path);
		free(temp_path);

		return -1;
	}

	if (rename(temp_path, todos_path) != 0) {
		fprintf(stderr,
				"Error: Unable to rename temp file to todos files (%s): %s\n",
				todos_path, strerror(errno));

		free(todos_path);
		free(temp_path);

		return -1;
	}

	free(todos_path);
	free(temp_path);

	view_todos();

	return 0;
}
