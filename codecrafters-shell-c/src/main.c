#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void find_cmd(char *cmd)
{
    /* Built-in commands */
    if (strcmp(cmd, "exit") == 0 ||
        strcmp(cmd, "echo") == 0 ||
        strcmp(cmd, "type") == 0)
    {
        printf("%s is a shell builtin\n\n", cmd);
        return;
    }

    /* Get PATH environment variable */
    char *path = getenv("PATH");

    if (path == NULL)
    {
        printf("%s: not found\n\n", cmd);
        return;
    }

    /* Make a copy of PATH */
    char path_copy[4096];
    strcpy(path_copy, path);

    /* Get first directory */
    char *dir = strtok(path_copy, ":");

    while (dir != NULL)
    {
        char fullpath[1024];

        /* Construct path */
        snprintf(fullpath, sizeof(fullpath),
                 "%s/%s", dir, cmd);

        /* Check if command exists and is executable */
        if (access(fullpath, X_OK) == 0)
        {
            printf("%s is %s\n\n", cmd, fullpath);
            return;
        }

        /* Next directory */
        dir = strtok(NULL, ":");
    }

    /* Command not found */
    printf("%s: not found\n\n", cmd);
}

int main()
{
    char input[100];

    while (1)
    {
        printf("\n$ ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        /* Remove newline */
        input[strcspn(input, "\r\n")] = '\0';

        /* exit */
        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        /* echo */
        else if (strncmp(input, "echo ", 5) == 0)
        {
            printf("%s\n", input + 5);
        }

        /* type */
        else if (strncmp(input, "type ", 5) == 0)
        {
            find_cmd(input + 5);
        }

        /* Unknown command */
        else
        {
            printf("%s: command not found\n\n", input);
        }
    }
    printf("Exiting shell...\n\n");
    return 0;
}
