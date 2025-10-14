/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:37:08 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:54:44 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	unsigned char	value;

	ptr = (unsigned char *) b;
	while (len > 0)
	{
		value = (unsigned char) c;
		*ptr = value;
		ptr++;
		len--;
	}
	return (b);
}

/* #include <stdio.h>

int main(void)
{
    char *s = "abcdefg";
    char c = 90;
    ft_memset(s, 'A', 3);
    printf("%s\n", s);
} */