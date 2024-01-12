#include "shell.h"
int hsh(info_t *information, char **argument_vector)
{
    ssize_t read_result = 0;
    int builtin_return = 0;

    while (read_result != -1 && builtin_return != -2)
    {
        clear_info(information);
        if (interactive(information))
            _puts("$ ");
        _eputchar(BUF_FLUSH);
        read_result = get_input(information);
        if (read_result != -1)
        {
            set_info(information, argument_vector);
            builtin_return = find_builtin(information);
            if (builtin_return == -1)
                find_cmd(information);
        }
        else if (interactive(information))
            _putchar('\n');
        free_info(information, 0);
    }
    write_history(information);
    free_info(information, 1);
    if (!interactive(information) && information->status)
        exit(information->status);
    if (builtin_return == -2)
    {
        if (information->err_num == -1)
            exit(information->status);
        exit(information->err_num);
    }
    return (builtin_return);
}

int find_builtin(info_t *information)
{
    int i, built_in_return = -1;
    builtin_table builtin_table[] = {
        {"exit", _myexit},
        {"env", _myenv},
        {"help", _myhelp},
        {"history", _myhistory},
        {"setenv", _mysetenv},
        {"unsetenv", _myunsetenv},
        {"cd", _mycd},
        {"alias", _myalias},
        {NULL, NULL}};

    for (i = 0; builtin_table[i].type; i++)
        if (_strcmp(information->argv[0], builtin_table[i].type) == 0)
        {
            information->line_count++;
            built_in_return = builtin_table[i].func(information);
            break;
        }
    return (built_in_return);
}

void find_cmd(info_t *information)
{
    char *path = NULL;
    int i, k;

    information->path = information->argv[0];
    if (information->linecount_flag == 1)
    

    {
        information->line_count++;
        information->linecount_flag = 0;
    }
    for (i = 0, k = 0; information->arg[i]; i++)
        if (!is_delim(information->arg[i], " \t\n"))
            k++;
    if (!k)
        return;

    path = find_path(information, _getenv(information, "PATH="), information->argv[0]);
    if (path)
    {
        information->path = path;
        fork_cmd(information);
    }
    else
    {
        if ((interactive(information) || _getenv(information, "PATH=") || information->argv[0][0] == '/') && is_cmd(information, information->argv[0]))
            fork_cmd(information);
        else if (*(information->arg) != '\n')
        {
            information->status = 127;
             
	}
}
}

