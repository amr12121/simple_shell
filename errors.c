#include "shell.h"

#include <stdlib.h>  

typedef struct {
    char *argument;
    char **arguments;
    char *path;
    int argument_count;
} info_t;

void clear_info(info_t *program_info)
{
    /* Use arrow operator (->) instead of dash (-) to access structure members*/
    program_info->argument = NULL;
    program_info->arguments = NULL;
    program_info->path = NULL;
    program_info->argument_count = 0;
}

void set_info(info_t *program_info, char **arguments_vector)
{
    int i = 0;

    /* Use arrow operator (->) instead of dash (-) to access structure members*/
    program_info->path = arguments_vector[0];
    if (program_info->path)
    {
        /* Assuming the functions exist and are correctly implemented*/
        program_info->arguments = strtow(program_info->path, " \t");
        if (!program_info->arguments)
        {
            program_info->arguments = (char **)malloc(sizeof(char *) * 2);
            if (program_info->arguments)
            {
                program_info->arguments[0] = _strdup(program_info->path);
                program_info->arguments[1] = NULL;
            }
        }
        for (i = 0; program_info->arguments && program_info->arguments[i]; i++)
            ;
        program_info->argument_count = i;

        /* Assuming the functions exist and are correctly implemented*/
        replace_alias(program_info);
        replace_variables(program_info);
    }
}

void free_info(info_t *program_info, int free_all)
{
    ffree(program_info->arguments);
    program_info->arguments = NULL;
    program_info->path = NULL;

    if (free_all)
    {
        if (!program_info->command_buffer)
            free(program_info->argument);

        /* Assuming the functions exist and are correctly implemented */
        if (program_info->environment)
            free_list(&(program_info->environment));
        if (program_info->history)
            free_list(&(program_info->history));
        if (program_info->alias)
            free_list(&(program_info->alias));

        ffree(program_info->environment);
        bfree((void **)program_info->command_buffer);
        if (program_info->read_file_descriptor > 2)
            close(program_info->read_file_descriptor);
    }
}
