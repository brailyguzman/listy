#include <string.h>

#include "../include/colors.h"
#include "../include/listy.h"
#include "../include/todo_io.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Since no extra arguments were provided, we logging our todos
        show_todos();
        return 0;
    }

    char *first_arg = argv[1];
    char input[100];

    if (strcmp(first_arg, "add") == 0) {
        printf("[%s?%s] Enter Text: ", YEL, RESET);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            input[strcspn(input, "\n")] = '\0';

            Todo newTodo = create_todo(input);
            printf("[%s+%s] %s%s%s\n", YEL, RESET, YEL, newTodo.text, RESET);
            return 0;
        } else {
            perror("Error reading input");
            return -1;
        }
    } else if (strcmp(first_arg, "remove") == 0) {
        // run the remove todo function, get the ID from the [2] index;
    } else if (strcmp(first_arg, "update") == 0) {
        // run the update todo function
    }
    return 0;
}