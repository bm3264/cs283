#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

int exec_local_cmd_loop()
{
    char cmd_line[SH_CMD_MAX];
    cmd_buff_t cmd_buff;
    int rc;

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_line, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }
        
        // Remove trailing newline
        cmd_line[strcspn(cmd_line, "\n")] = '\0';
        
        // Trim leading and trailing spaces
        char *start = cmd_line;
        while (*start == SPACE_CHAR) start++;
        char *end = start + strlen(start) - 1;
        while (end > start && *end == SPACE_CHAR) *end-- = '\0';

        // Ignore empty input
        if (*start == '\0')
        {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        // Parse command into cmd_buff structure
        clear_cmd_buff(&cmd_buff);
        rc = build_cmd_buff(start, &cmd_buff);
        if (rc != OK)
        {
            if (rc == ERR_TOO_MANY_COMMANDS)
                printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }

        // Check for built-in commands
        Built_In_Cmds cmd_type = match_command(cmd_buff.argv[0]);
        if (cmd_type != BI_NOT_BI)
        {
            if (cmd_type == BI_CMD_EXIT)
                exit(0);
            else if (cmd_type == BI_CMD_CD)
            {
                if (cmd_buff.argc > 1)
                {
                    if (chdir(cmd_buff.argv[1]) != 0)
                        perror("cd");
                }
                continue;
            }
            exec_built_in_cmd(&cmd_buff);
            continue;
        }

        // Fork and execute external command
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }
        else if (pid == 0)
        {
            execvp(cmd_buff.argv[0], cmd_buff.argv);
            perror("execvp");
            exit(ERR_EXEC_CMD);
        }
        else
        {
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return OK;
}