#include "todo_io.h"

char* get_path() {
	const char* home = getenv("HOME");

	if (home == NULL) {
		fprintf(stderr, "Error: Home environment variable is not set.\n");
		return NULL;
	}

	const char* dir_name = "/listy";
	const char* listy_file = "/todos.txt";

	// Allocate memory for the directory path.
	size_t dir_path_len = strlen(home) + strlen(dir_name) + 1;
	char* dir_path = malloc(dir_path_len);

	if (dir_path == NULL) {
		fprintf(stderr, "Error: Memory allocation failed for dir_path.\n");

		return NULL;
	}

	snprintf(dir_path, dir_path_len, "%s%s", home, dir_name);

	DIR* dir = opendir(dir_path);

	if (dir) {
		// If the directory exists, simply close it.
		closedir(dir);
	} else {
		// If the directory does not exist, create it, and do error handling.
		if (mkdir(dir_path, 0777) != 0) {
			perror("Failed to create directory");
			free(dir_path);

			return NULL;
		}
	}

	size_t full_path_len = strlen(dir_path) + strlen(listy_file) + 1;
	char* full_path = malloc(full_path_len);

	if (full_path == NULL) {
		fprintf(stderr, "Error: Memory allocation failed.\n");
		free(dir_path);

		return NULL;
	}

	snprintf(full_path, full_path_len, "%s%s", dir_path, listy_file);

	// Release the memory allocated for dir path.
	free(dir_path);

	return full_path;
}