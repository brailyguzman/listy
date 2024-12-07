#include <colors.h>
#include <listy.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc <= 1) {
		view_todos();
		return 0;
	}

	if (strstr(argv[1], "help")) {
		// TODO: Help command should display available commands.
		return 0;
	}

	if (strstr(argv[1], "add")) {
		if (argv[2] != NULL) {
			int result = add_todo(argv[2]);

			if (result == -1) {
				return -1;
			}
		} else {
			char text[TASK_SIZE];
			printf("%s[?]%s Enter Todo: ", YEL, RESET);

			if (fgets(text, sizeof(text), stdin) != NULL) {
				int result = add_todo(text);

				if (result == -1) {
					return -1;
				}
			}
		}

		return 0;
	}

	if (strstr(argv[1], "remove")) {
		if (argv[2] != NULL) {
			int id = atoi(argv[2]);

			if (id >= 0) {
				edit_todo(id, 1);
			} else {
				fprintf(
					stderr,
					"Error: Invalid ID. ID must be a non-negative integer.\n");
			}
		} else {
			fprintf(stderr,
					"Error: Missing ID argument for 'remove' command.\n");
		}
	}

	if (strstr(argv[1], "done")) {
		if (argv[2] != NULL) {
			int id = atoi(argv[2]);

			if (id >= 0) {
				edit_todo(id, 0);
			} else {
				fprintf(
					stderr,
					"Error: Invalid ID. ID must be a non-negative integer.\n");
			}
		} else {
			fprintf(stderr,
					"Error: Missing ID argument for 'remove' command.\n");
		}
	}

	return 0;
}