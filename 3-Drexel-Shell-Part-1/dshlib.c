#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (cmd_line == NULL || strlen(cmd_line) == 0)
    {
        return WARN_NO_CMDS;
    }

    char *token;
    char *rest = cmd_line;
    int cmd_count = 0;

    while ((token = strtok_r(rest, PIPE_STRING, &rest)) != NULL)
    {
        if (cmd_count >= CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }

        // Trim leading spaces
        while (*token == SPACE_CHAR)
            token++;

        // Ensure command isn't empty
        if (*token == '\0')
        {
            return WARN_NO_CMDS;
        }

        // Extract command and arguments
        char *arg_start = strchr(token, SPACE_CHAR);
        if (arg_start)
        {
            *arg_start = '\0';
            arg_start++;
            while (*arg_start == SPACE_CHAR)
                arg_start++;
            strncpy(clist->commands[cmd_count].args, arg_start, ARG_MAX - 1);
        }

        strncpy(clist->commands[cmd_count].exe, token, EXE_MAX - 1);
        clist->num++;
        cmd_count++;
    }
    return OK;
}