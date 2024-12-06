#include <listy.h>

void view_todos() {
	char* path = get_path();

	FILE* fptr = fopen(path, "r");

	char line[256];
	char* delimiter = "|";

	// Print header with appropriate widths
	printf("\n%s%-5s %-40s %-20s%s\n", GRN, "ID", "Task", "Status", RESET);
	printf("%s%-5s %-40s %-20s%s\n", GRN, "-----",
		   "----------------------------------------", "-------------", RESET);

	while (fgets(line, sizeof(line), fptr)) {
		int count = 0;
		char* token = strtok(line, delimiter);

		char id[16] = "";
		char task[TASK_SIZE] = "";
		char status[32] = "";

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

	char line[256];
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

int add(char* text) {
	char* path = get_path();

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

	char line[256];

	text[strcspn(text, "\n")] = '\0';

	snprintf(line, sizeof(line), "%d|%s|%d\n", id, text, 0);
	fputs(line, fptr);

	free(path);
	fclose(fptr);

	return 0;
}