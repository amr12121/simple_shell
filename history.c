#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

char *get_history_file(ino_t *info)
{
    char *dir = _getenv(info, "HOME=");
    if (!dir || !(*dir))  /* Check if dir is empty or NULL*/
    {
        free(dir);
        return NULL;
    }

    size_t filename_len = _strlen(dir) + _strlen(HIST_FILE) + 2;
    char *filename = malloc(filename_len);
    if (!filename)
    {
        free(dir);
        return NULL;
    }

    /* Use snprintf for string concatenation*/
    snprintf(filename, filename_len, "%s/%s", dir, HIST_FILE);
    free(dir);

    return filename;
}

void write_history(ino_t *info)
{
    char *filename = get_history_file(info);
    if (!filename)
        return;

    int fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);

    if (fd == -1)
    {
        perror("Error opening history file");
        return;
    }

    list_t *node = info->history;
    for (; node; node = node->next)
        dprintf(fd, "%s\n", node->str);

    close(fd);
}


/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_history(ino_t *info)
{
	char *filename = get_history_file(info);
	if (!filename)
		return -1;

	int fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);

	if (fd == -1)
		return -1;

	list_t *node = info->history;
	while (node)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
		node = node->next;
	}

	_putfd(BUF_FLUSH, fd);
	close(fd);
	return 1;
}

/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_history(ino_t *info)
{
	char *filename = get_history_file(info);
	if (!filename)
		return 0;

	int fd = open(filename, O_RDONLY);
	free(filename);

	if (fd == -1)
		return 0;

	struct stat st;
	if (fstat(fd, &st))
	{
		close(fd);
		return 0;
	}

	if (st.st_size < 2)
	{
		close(fd);
		return 0;
	}

	char *buf = malloc(st.st_size + 1);
	if (!buf)
	{
		close(fd);
		return 0;
	}

	ssize_t rdlen = read(fd, buf, st.st_size);
	close(fd);

	if (rdlen <= 0)
	{
		free(buf);
		return 0;
	}

	buf[st.st_size] = '\0';

	int last = 0, linecount = 0;
	for (int i = 0; i < st.st_size; i++)
	{
		if (buf[i] == '\n')
		{
			buf[i] = '\0';
			build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	}

	if (last != st.st_size)
		build_history_list(info, buf + last, linecount++);

	free(buf);
	info->histcount = linecount;

	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);

	renumber_history(info);
	return info->histcount;
}

/**
 * build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int build_history_list(ino_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;

	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;

	return 0;
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(ino_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}

	return (info->histcount = i);
}

