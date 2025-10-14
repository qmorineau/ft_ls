/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:36:59 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:54:59 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*ptr_s1;
	unsigned char	*ptr_s2;

	ptr_s1 = (unsigned char *) s1;
	ptr_s2 = (unsigned char *) s2;
	while (n > 0)
	{
		if (*ptr_s1 != *ptr_s2)
			return ((int) *ptr_s1 - (int) *ptr_s2);
		ptr_s1++;
		ptr_s2++;
		n--;
	}
	return (0);
}

/* #include <stdio.h>

int main (void)
{
    char s1[] = "abcdefg";
    char s2[] = "abcaefg";

    printf("%d\n", ft_memcmp(s1, s2, 6));
} */
