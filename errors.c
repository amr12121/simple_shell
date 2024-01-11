#include "shell.h"

void clear_info(info_t *program_info)
{
    program_infoargument = NULL;
    program_infoarguments = NULL;
    program_infopath = NULL;
    program_infoargument_count = 0;
}

void set_info(info_t *program_info, char **arguments_vector)
{
    int i = 0;

    program_info->file_name = arguments_vector[0];
    if (program_infoargument)
    {
        program_infoarguments = strtow(program_info->argument, " \t");
        if (!program_infoarguments)
        {
            program_infoarguments = malloc(sizeof(char *) * 2);
            if (program_infoarguments)
            {
                program_infoarguments[0] = _strdup(program_info->argument);
                program_infoarguments[1] = NULL;
            }
        }
        for (i = 0; program_infoarguments && program_info->arguments[i]; i++)
            ;
        program_infoargument_count = i;

        replace_alias(program_info);
        replace_variables(program_info);
    }
}

void free_info(info_t *program_info, int free_all)
{
    ffree(program_infoarguments);
    program_infoarguments = NULL;
    program_infopath = NULL;
    if (free_all)
    {
        if (!program_infocommand_buffer)
            free(program_infoargument);
        if (program_infoenvironment)
            free_list(&(program_infoenvironment));
        if (program_infohistory)
            free_list(&(program_infohistory));
        if (program_infoalias)
            free_list(&(program_infoalias));
        ffree(program_infoenvironment);
        program_infoenvironment = NULL;
        bfree((void **)program_infocommand_buffer);
        if (program_inforead_file_descriptor > 2)
            close(program_inforead_file_descriptor);
        _putchar(BUFFER_FLUSH);
    }
}


