#include "shell.h"


char *_memset(char *memory_area, char fill_byte, unsigned int bytes_to_fill)
{
    unsigned int i;

    for (i = 0; i < bytes_to_fill; i++)
        memory_area[i] = fill_byte;
    return memory_area;
}

void ffree(char **string_of_strings)
{
    char **current_ptr = string_of_strings;

    if (!string_of_strings)
        return;
    while (*string_of_strings)
        free(*string_of_strings++);
    free(current_ptr);
}

void *_realloc(void *previous_block, unsigned int old_size, unsigned int new_size)
{
    char *new_block;

    if (!previous_block)
        return malloc(new_size);
    if (!new_size)
        return (free(previous_block), NULL);
    if (new_size == old_size)
        return previous_block;

    new_block = malloc(new_size);
    if (!new_block)
        return NULL;

    old_size = old_size < new_size ? old_size : new_size;
    while (old_size--)
        new_block[old_size] = ((char *)previous_block)[old_size];
    free(previous_block);
    return new_block;
}

