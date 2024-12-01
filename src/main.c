#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc <= 1 || strcmp(argv[1], "help") == 0) {
		// Here, we should print how to use the available commands.
		return 0;
	}

	if (strcmp(argv[1], "add") == 0) {
		// TODO: Implement add todo functionality
	}

	return 0;
}