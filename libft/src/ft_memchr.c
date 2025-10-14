/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:36:55 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:55:04 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr;
	unsigned char	value;

	ptr = (unsigned char *) s;
	value = (unsigned char) c;
	while (n > 0)
	{
		if (value == *ptr)
			return (ptr);
		ptr++;
		n--;
	}
	return (NULL);
}

/* #include <stdio.h>

int main(void)
{
    char s[] = "abcdefghikl";
    char c = 'c';

    printf("%s\n", s);
    char *s1 = ft_memchr(s, c, 4);
    printf("%s\n", s1);
} */