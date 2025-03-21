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

        int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
            if (!cmd_line || !cmd_buff) return ERR_MEMORY;
        
            cmd_buff->_cmd_buffer = strdup(cmd_line); // holds entire command
            if (!cmd_buff->_cmd_buffer) return ERR_MEMORY;
        
            int argc = 0;
            char *token = cmd_buff->_cmd_buffer;
            bool in_quotes = false;
            char *start = token;
        
            while (*token) {
                // Skip leading spaces
                while (*token == SPACE_CHAR) token++;
        
                if (*token == '\"') {
                    token++;  // skip opening quote
                    start = token;
                    while (*token && *token != '\"') token++;
                } else {
                    start = token;
                    while (*token && *token != SPACE_CHAR) token++;
                }
        
                if (*token) {
                    *token = '\0';  // terminate token
                    token++;
                }
        
                if (argc >= CMD_ARGV_MAX - 1) return ERR_CMD_ARGS_BAD;
        
                cmd_buff->argv[argc++] = start;
            }
        
            cmd_buff->argv[argc] = NULL;
            cmd_buff->argc = argc;
            return OK;
        }        

        // Parse command into cmd_buff structure
        int clear_cmd_buff(cmd_buff_t *cmd_buff) {
            if (!cmd_buff) return ERR_MEMORY;
        
            for (int i = 0; i < CMD_ARGV_MAX; i++) {
                cmd_buff->argv[i] = NULL;
            }
        
            if (cmd_buff->_cmd_buffer) {
                free(cmd_buff->_cmd_buffer);
                cmd_buff->_cmd_buffer = NULL;
            }
        
            cmd_buff->argc = 0;
            return OK;
        }
        
        Built_In_Cmds match_command(const char *input) {
            if (strcmp(input, "exit") == 0) return BI_CMD_EXIT;
            if (strcmp(input, "cd") == 0) return BI_CMD_CD;
            return BI_NOT_BI;
        }

        Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
            if (!cmd || cmd->argc == 0) return BI_NOT_BI;
        
            Built_In_Cmds type = match_command(cmd->argv[0]);
        
            if (type == BI_CMD_CD) {
                if (cmd->argc > 1) {
                    if (chdir(cmd->argv[1]) != 0) {
                        perror("cd");
                    }
                }
                return BI_EXECUTED;
            } else if (type == BI_CMD_EXIT) {
                exit(0);
            }
        
            return BI_NOT_BI;
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