#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dshlib.h"

int main()
{
    char cmd_buff[SH_CMD_MAX];
    command_list_t clist;
    int rc;

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // Remove trailing newline
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Check for exit command
        if (strcmp(cmd_buff, EXIT_CMD) == 0)
        {
            exit(0);
        }

        // Parse the command
        memset(&clist, 0, sizeof(command_list_t));
        rc = build_cmd_list(cmd_buff, &clist);

        // Handle command parsing results
        if (rc == WARN_NO_CMDS)
        {
            printf(CMD_WARN_NO_CMD);
        }
        else if (rc == ERR_TOO_MANY_COMMANDS)
        {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        }
        else if (rc == OK)
        {
            printf(CMD_OK_HEADER, clist.num);
            for (int i = 0; i < clist.num; i++)
            {
                if (strlen(clist.commands[i].args) > 0)
                {
                    printf("<%d> %s [%s]\n", i + 1, clist.commands[i].exe, clist.commands[i].args);
                }
                else
                {
                    printf("<%d> %s\n", i + 1, clist.commands[i].exe);
                }
            }
        }
    }
    return 0;
}
