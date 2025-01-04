#include <colors.h>
#include <listy.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *title;
    char *description;
} Command;

void help() {
    Command commands[] = {
        {.title = "no flag", .description = "Shows the available todos."},
        {.title = "add <text>",
         .description = "Adds a new todo to the list with the given text."},
        {.title = "remove <id>",
         .description = "Removes the todo with the given ID from the list."},
        {.title = "done <id>",
         .description = "Marks the todo with the given ID as complete."},
        {.title = "clear", .description = "Removes all todos from the list."}};

    size_t length = sizeof(commands) / sizeof(commands[0]);

    for (size_t i = 0; i < length; i++) {
        Command command = commands[i];
        printf("%s%s%s\n  - %s%s%s\n", GRN, BOLD, command.title, RESET, CYN,
               command.description);
    }
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        view_todos();
    } else if (strstr(argv[1], "help")) {
        help();
    } else if (strstr(argv[1], "clear")) {
        clear_todos();
    } else if (strstr(argv[1], "add")) {
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
    } else if (strstr(argv[1], "remove")) {
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
    } else if (strstr(argv[1], "done")) {
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
    } else {
        printf("%sInvalid flag%s\n\n", RED, RESET);
        help();
    }

    return 0;
}
