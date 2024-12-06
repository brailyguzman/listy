#include <colors.h>
#include <listy.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc <= 1) {
		// Todo: Display the current todos.
		return 0;
	}

	if (strcmp(argv[1], "help") == 0) {
		// Todo: Display a guide on how to use the commands.
		return 0;
	}

	if (strcmp(argv[1], "add") == 0) {
		char text[100];
		printf("%s[?]%s Enter Todo: ", YEL, RESET);

		if (fgets(text, sizeof(text), stdin) != NULL) {
			int result = add(text);

			if (result == -1) {
				return -1;
			}

			printf("%s[+]%s Added Todo!\n", GRN, RESET);

			return 0;
		}
	}

	// TODO: Finish implement CRUD for Todos.

	return 0;
}