#include <listy.h>

int get_new_id(char* path) {
	FILE* fptr = fopen(path, "r");

	if (fptr == NULL) {
		fprintf(stderr, "Error: Unable to open the todos file.");

		return -1;
	}

	char line[256];
	int max_id = 0;

	while (fgets(line, sizeof(line), fptr)) {
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

	FILE* fptr;
	fptr = fopen(path, "a");

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

	// sanitize the text by removing newline if it exists
	text[strcspn(text, "\n")] = 0;

	snprintf(line, sizeof(line), "%d|%s|%d\n", id, text, 1);

	fputs(line, fptr);

	free(path);
	fclose(fptr);

	return 0;
}