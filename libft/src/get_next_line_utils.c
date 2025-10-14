/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 18:30:42 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/24 14:29:13 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	strclen(const char *s, char c)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == '\n' && c == '\n')
		i++;
	return (i);
}

char	*strjoin_and_free(char *s1, char *s2)
{
	int		s1_len;
	int		s2_len;
	char	*string;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return ((char *) s2);
	if (!s2)
		return ((char *) s1);
	s1_len = strclen(s1, '\0');
	s2_len = strclen(s2, '\0');
	string = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!string)
		return (free_str(&s1), free_str(&s2), NULL);
	ft_memcpy(string, s1, s1_len);
	ft_memcpy(&string[s1_len], s2, s2_len + 1);
	return (free_str(&s1), free_str(&s2), string);
}

char	*ft_strndup(const char *s, size_t len)
{
	char	*ptr;

	if (!s)
		return (NULL);
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s, len);
	ptr[len] = '\0';
	return (ptr);
}
