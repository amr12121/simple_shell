#include "shell.h"


char *_strcpy(char *destination, char *source)
{
    int index = 0;

    if (destination == source || source == NULL)
        return destination;
    while (source[index])
    {
        destination[index] = source[index];
        index++;
    }
    destination[index] = 0;
    return destination;
}

char *_strdup(const char *str)
{
    int length = 0;
    char *result;

    if (str == NULL)
        return NULL;
    while (*str++)
        length++;
    result = malloc(sizeof(char) * (length + 1));
    if (!result)
        return NULL;
    for (length++; length--;)
        result[length] = *--str;
    return result;
}

void _puts(char *str)
{
    int index = 0;

    if (!str)
        return;
    while (str[index] != '\0')
    {
        _putchar(str[index]);
        index++;
    }
}

int _putchar(char character)
{
    static int i;
    static char buffer[WRITE_BUF_SIZE];

    if (character == BUF_FLUSH || i >= WRITE_BUF_SIZE)
    {
        write(1, buffer, i);
        i = 0;
    }
    if (character != BUF_FLUSH)
        buffer[i++] = character;
    return 1;
}

