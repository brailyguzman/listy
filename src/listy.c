#include <listy.h>

void view_todos() {
	char* path = get_path("/todos.txt");

	if (path == NULL) {
		fprintf(stderr, "Error: Failed to get file path.\n");
		return;
	}

	FILE* fptr = fopen(path, "r");

	if (fptr == NULL) {
		fprintf(stderr, "Error: Unable to open todos file (%s).\n", path);
		free(path);

		return;
	}

	int c = fgetc(fptr);

	if (c == EOF) {
		fclose(fptr);
		free(path);
		printf("No todos to show!\n");

		return;
	}

	char line[LINE_LENGTH];
	char* delimiter = "|";

	// Print header with appropriate widths
	printf("\n%s%-5s %-40s %-20s%s\n", GRN, "ID", "Task", "Status", RESET);
	printf("%s%-5s %-40s %-20s%s\n", GRN, "-----",
		   "----------------------------------------", "-------------", RESET);

	while (fgets(line, sizeof(line), fptr)) {
		if (line[0] == '\0' || line[0] == '\n') {
			continue;
		}

		int count = 0;

		char* token = strtok(line, delimiter);
		char id[ID_SIZE] = "";
		char task[TASK_SIZE] = "";
		char status[STATUS_SIZE] = "";

		while (token != NULL) {
			switch (count) {
				case 0:
					strncpy(id, token, sizeof(id) - 1);
					break;
				case 1:
					strncpy(task, token, sizeof(task) - 1);
					break;
				case 2:
					if (atoi(token) == 0) {
						snprintf(status, sizeof(status), "%s✅ Completed%s",
								 GRN, RESET);
					} else if (atoi(token) == 1) {
						snprintf(status, sizeof(status), "%s❌ Incomplete%s",
								 MAG, RESET);
					}
					break;
				default:
					break;
			}

			count++;
			token = strtok(NULL, delimiter);
		}

		if (strlen(task) > MAX_TASK_LENGTH) {
			task[MAX_TASK_LENGTH] = '\0';
			strcat(task, "...");
		}

		printf("%-5s %-40s %-20s\n", id, task, status);
	}

	printf("\n");

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

	int id = get_new_id(path);

	if (id == -1) {
		return -1;
	}

	char line[LINE_LENGTH];

	text[strcspn(text, "\n")] = '\0';

	snprintf(line, sizeof(line), "%d|%s|%d\n", id, text, 1);
	fputs(line, fptr);

	free(path);
	fclose(fptr);

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

			fprintf(temp_fptr, "%s|%s|%d", id_str, line_task, newStatus);
		} else {
			fprintf(temp_fptr, "%s|%s|%s", id_str, line_task, status_str);
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

	return 0;
}
