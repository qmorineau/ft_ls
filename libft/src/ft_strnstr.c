/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:38:53 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 11:40:38 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	little_len;

	little_len = 0;
	while (needle[little_len])
		little_len++;
	if (little_len == 0)
		return ((char *) haystack);
	if (len == 0)
		return (NULL);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (haystack[i + j] == needle[j] && len > i + j)
		{
			j++;
			if (needle[j] == '\0')
				return ((char *) &haystack[i]);
		}
		i++;
	}
	return (NULL);
}

/* #include <string.h>
#include <stdio.h>

int main (void)
{
	const char *largestring = "abcdef";
	const char *smallstring = "abcdefghijklmno";
    char *ptr;

    ptr = ft_strnstr(largestring, smallstring, 6);
	if (ptr != NULL) {
		printf("%s\n", ptr);
	} else {
		printf("null\n");
	}
} */