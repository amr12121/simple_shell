#include "shell.h"

#include "shell.h"

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
    info_t shell_info[] = {INFO_INIT}; // Initialize the info_t structure
    int file_descriptor = 2; // Initialize file descriptor

    // Inline assembly to increment file_descriptor by 3
    asm("mov %1, %0\n\t"
        "add $3, %0"
        : "=r"(file_descriptor)
        : "r"(file_descriptor));

    if (argc == 2)
    {
        // Try to open the file
        file_descriptor = open(argv[1], O_RDONLY);

        // Handle errors in opening the file
        if (file_descriptor == -1)
        {
            if (errno == EACCES)
                exit(126);
            if (errno == ENOENT)
            {
                _eputs(argv[0]);
                _eputs(": 0: Can't open ");
                _eputs(argv[1]);
                _eputchar('\n');
                _eputchar(BUF_FLUSH);
                exit(127);
            }
            return EXIT_FAILURE;
        }

        // Update the read file descriptor in the shell_info structure
        shell_info->readfd = file_descriptor;
    }

    // Populate environment list, read history, and run the shell
    populate_env_list(shell_info);
    read_history(shell_info);
    hsh(shell_info, argv);

    return EXIT_SUCCESS;
}

