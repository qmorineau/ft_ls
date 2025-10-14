/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:38:36 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:59:09 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static size_t	ft_join_length(char const *s1, char const *s2)
{
	size_t	length;

	length = ft_strlen((char *)s1);
	length += ft_strlen((char *)s2);
	length++;
	return (length);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*string;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return ((char *) s2);
	if (!s2)
		return ((char *) s1);
	string = malloc(sizeof(char) * ft_join_length(s1, s2));
	if (!string)
		return (NULL);
	ft_strcpy(string, s1);
	ft_strcpy(&string[ft_strlen((char *) s1)], s2);
	return (string);
}

/* #include <stdio.h>

int main(void)
{
	char s1[] = "abcd";
	char s2[] = "efgh";
	printf("%s\n", ft_strjoin(s1, s2));
} */