#include "shell.h"





int is_cmd(info_t *info, char *file_path)
{
    struct stat file_stat;

    (void)info;
    if (!file_path || stat(file_path, &file_stat))
        return (0);

    if (file_stat.st_mode & S_IFREG)
    {
        return (1);
    }
    return (0);
}

char *dup_chars(char *source_str, int start, int stop)
{
    static char buffer[1024];
    int i = 0, k = 0;

    for (k = 0, i = start; i < stop; i++)
        if (source_str[i] != ':')
            buffer[k++] = source_str[i];
    buffer[k] = 0;
    return (buffer);
}

char *find_path(info_t *info, char *path_string, char *command)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!path_string)
        return (NULL);
    if ((_strlen(command) > 2) && starts_with(command, "./"))
    {
        if (is_cmd(info, command))
            return (command);
    }
    while (1)
    {
        if (!path_string[i] || path_string[i] == ':')
        {
            path = dup_chars(path_string, curr_pos, i);
            if (!*path)
                _strcat(path, command);
            else
            {
                _strcat(path, "/");
                _strcat(path, command);
            }
            if (is_cmd(info, path))
                return (path);
            if (!path_string[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return (NULL);
}

