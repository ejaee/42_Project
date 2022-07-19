/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ejachoi <ejachoi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 10:04:31 by choiejae          #+#    #+#             */
/*   Updated: 2022/07/19 15:01:09 by ejachoi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	is_newline(char *s)
{
	int	idx_newline;

	idx_newline = -1;
	while (s[++idx_newline])
	{
		if (s[idx_newline] == '\n')
			return (idx_newline);
	}
	return (-1);
}

static char *split_to_line(char **backup, char *buf)
{
	char	*line;
	char	*temp_new_backup;
	int		idx_newline;

	line = NULL;
	idx_newline = is_newline(*backup);
	if (idx_newline == -1)
	{
		if (*backup[0] != '\0')
			line = ft_strndup(*backup, 0, ft_strlen(*backup));
		free(*backup);
		*backup = NULL;
	}
	else
	{
		line = ft_strndup(*backup, 0, idx_newline);
		temp_new_backup = ft_strndup(*backup, idx_newline + 1, ft_strlen(*backup) - idx_newline - 1);
		free(*backup);
		*backup = temp_new_backup;
	}
	free(buf);
	return (line);
}

static char *get_line(int fd, char **backup, char *buf)
{
	char	*new_backup;
	int		read_byte;
	int		idx_next;

	read_byte = read(fd, buf, BUFFER_SIZE);
	while (read_byte > 0)
	{
		buf[read_byte] = '\0';
		new_backup = ft_strjoin(*backup, buf);
		free(*backup);
		*backup = new_backup;
		idx_next = is_newline(*backup);
		if (idx_next != -1)
			return (split_to_line(backup, buf));
		read_byte = read(fd, buf, BUFFER_SIZE);
	}
	return (split_to_line(backup, buf));
}

char *get_next_line(int fd)
{
	char	*buf;
	static char	*backup;

	buf = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (buf == NULL)
		return (NULL);
	if (BUFFER_SIZE <= 0 && read(fd, buf, 0) == -1)
	{
		free(buf);
		return (NULL);
	}
	if (backup != NULL && is_newline(backup) != -1)
		return (split_to_line(&backup, buf));
	if (backup == NULL)
		backup = ft_strndup("", 0, 0);
	return (get_line(fd, &backup, buf));
}
