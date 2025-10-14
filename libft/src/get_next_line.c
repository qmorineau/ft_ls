/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: quentin <quentin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 18:30:40 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/25 11:51:09 by quentin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_str(char **s)
{
	if (s && *s)
	{
		free(*s);
		*s = NULL;
	}
	*s = NULL;
}

char	*read_and_free(char *tmp, char *text, int fd)
{
	ssize_t	byte_read;

	byte_read = 1;
	while (byte_read > 0)
	{
		tmp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!tmp)
			return (free_str(&text), NULL);
		byte_read = read(fd, tmp, BUFFER_SIZE);
		if (byte_read < 0)
			return (free_str(&tmp), free_str(&text), NULL);
		if (byte_read == 0)
		{
			if (strclen(text, '\0') == 0)
				return (free_str(&tmp), free_str(&text), NULL);
			return (free_str(&tmp), text);
		}
		tmp[byte_read] = '\0';
		text = strjoin_and_free(text, tmp);
		if (!text)
			return (NULL);
		if (ft_strchr(text, '\n'))
			break ;
	}
	return (text);
}

char	*check_file_and_static(char **text, int fd)
{
	if (read(fd, 0, 0) < 0)
		return (free_str(&text[fd]), NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE >= INT_MAX
		|| read(fd, 0, 0) < 0 || fd > OPEN_MAX)
	{
		free_str(text);
		return (NULL);
	}
	if (!text[fd])
	{
		text[fd] = malloc(sizeof(char) * 1);
		if (!text[fd])
			return (NULL);
		text[fd][0] = '\0';
	}
	return (text[fd]);
}

char	*get_next_line(int fd)
{
	static char	*text[OPEN_MAX] = {NULL};
	char		*string;
	int			len;
	char		*tmp;

	text[fd] = check_file_and_static(text, fd);
	if (!text[fd])
		return (free_str(&text[fd]), NULL);
	if (!ft_strchr(text[fd], '\n'))
	{
		tmp = NULL;
		text[fd] = read_and_free(tmp, text[fd], fd);
		if (!text[fd])
			return (free_str(&text[fd]), NULL);
	}
	len = strclen(text[fd], '\n');
	string = ft_strndup(text[fd], len);
	tmp = ft_strndup(&text[fd][len], strclen(text[fd], '\0') - len);
	free_str(&text[fd]);
	text[fd] = ft_strndup(tmp, strclen(tmp, 0));
	free_str(&tmp);
	if (!string || !text[fd])
		return (free_str(&string), free_str(&text[fd]), NULL);
	return (string);
}

/* #include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int	a;
	int b;
	char *string;

	a = open("get_next_line.c", O_RDONLY);
	b = open("get_next_line_utils.c", O_RDONLY);
	printf("let'sgooooooo!\n");
	if (a < 0 || b < 0)
	{
		return (1);
	}
	string = get_next_line(a);
	printf("1 = %s\n", string);
	free_str(&string);
	string = get_next_line(b);
	printf("2 = %s\n", string);
	free_str(&string);
	string = get_next_line(a);
	printf("1 = %s\n", string);
	free_str(&string);
	string = get_next_line(b);
	printf("2 = %s\n", string);
	free_str(&string);
	close(a);
	close(b);
	return (0);
} */