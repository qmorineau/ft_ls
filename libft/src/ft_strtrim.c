/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:38:58 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 18:01:49 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_charset(char c, char const *set)
{
	size_t	i;

	i = 0;
	if (!set || set[0] == '\0')
		return (0);
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static size_t	ft_count_charset_start(char const *s1, char const *set)
{
	size_t	count;

	count = 0;
	while (ft_is_charset(s1[count], set))
		count++;
	return (count);
}

static size_t	ft_count_charset_end(char const *s1, char const *set)
{
	size_t	i;
	size_t	count;

	i = 0;
	while (s1[i])
		i++;
	i--;
	count = 0;
	while (i > 0 && ft_is_charset(s1[i], set))
	{
		count++;
		i--;
	}
	if (i == 0 && ft_is_charset(s1[i], set))
		count++;
	return (count);
}

static size_t	ft_trim_length(char const *s1, char const *set)
{
	size_t	length;
	size_t	start_trim;
	size_t	end_trim;

	length = 0;
	while (s1[length])
		length++;
	start_trim = ft_count_charset_start(s1, set);
	end_trim = ft_count_charset_end(s1, set);
	if (start_trim + end_trim >= length)
		return (0);
	return (length - start_trim - end_trim);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	char	*string;
	size_t	len;

	if (!s1)
		return (NULL);
	len = ft_trim_length(s1, set);
	string = malloc(sizeof(char) * (len + 1));
	if (!string)
		return (NULL);
	i = 0;
	j = 0;
	while (ft_is_charset(s1[i], set))
		i++;
	while (s1[i] && len > 0)
	{
		string[j] = s1[i];
		i++;
		j++;
		len--;
	}
	string[j] = '\0';
	return (string);
}

/* #include <stdio.h>

int main(void)
{
	char s[] = "   aaaaa    aa  abcde        aaaaa  ";
	char set[] = "";

	printf("%s\n", ft_strtrim(s, set));
} */