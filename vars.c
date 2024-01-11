#include "shell.h"




int is_chain(info_t *information, char *buffer, size_t *position)
{
    size_t j = *position;

    if (buffer[j] == '|' && buffer[j + 1] == '|')
    {
        buffer[j] = 0;
        j++;
        information->cmd_buf_type = CMD_OR;
    }
    else if (buffer[j] == '&' && buffer[j + 1] == '&')
    {
        buffer[j] = 0;
        j++;
        information->cmd_buf_type = CMD_AND;
    }
    else if (buffer[j] == ';') /* found end of this command */
    {
        buffer[j] = 0; /* replace semicolon with null */
        information->cmd_buf_type = CMD_CHAIN;
    }
    else
        return (0);
    *position = j;
    return (1);
}

void check_chain(info_t *information, char *buffer, size_t *position, size_t start, size_t length)
{
    size_t j = *position;

    if (information->cmd_buf_type == CMD_AND)
    {
        if (information->status)
        {
            buffer[start] = 0;
            j = length;
        }
    }
    if (information->cmd_buf_type == CMD_OR)
    {
        if (!information->status)
        {
            buffer[start] = 0;
            j = length;
        }
    }

    *position = j;
}

int replace_alias(info_t *information)
{
    int i;
    list_t *node;
    char *pointer;

    for (i = 0; i < 10; i++)
    {
        node = node_starts_with(information->alias, information->argv[0], '=');
        if (!node)
            return (0);
        free(information->argv[0]);
        pointer = _strchr(node->str, '=');
        if (!pointer)
            return (0);
        pointer = _strdup(pointer + 1);
        if (!pointer)
            return (0);
        information->argv[0] = pointer;
    }
    return (1);
}

int replace_vars(info_t *information)
{
    int i = 0;
    list_t *node;

    for (i = 0; information->argv[i]; i++)
    {
        if (information->argv[i][0] != '$' || !information->argv[i][1])
            continue;

        if (!_strcmp(information->argv[i], "$?"))
        {
            replace_string(&(information->argv[i]),
                            _strdup(convert_number(information->status, 10, 0)));
            continue;
        }
        if (!_strcmp(information->argv[i], "$$"))
        {
            replace_string(&(information->argv[i]),
                            _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(information->env, &information->argv[i][1], '=');
        if (node)
        {
            replace_string(&(information->argv[i]),
                            _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&information->argv[i], _strdup(""));
    }
    return (0);
}

int replace_string(char **old_string, char *new_string)
{
    free(*old_string);
    *old_string = new_string;
    return (1);
}

