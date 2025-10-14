/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:37:03 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:54:56 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*ptr_dest;
	const unsigned char	*ptr_src;

	if ((!dst && !src) || n == 0)
		return (dst);
	ptr_src = (const unsigned char *) src;
	ptr_dest = (unsigned char *) dst;
	while (n > 0)
	{
		*ptr_dest = *ptr_src;
		ptr_dest++;
		ptr_src++;
		n--;
	}
	return (dst);
}

/* #include <stdio.h>

int main(void) {
	char *s1 = NULL;
	char *s2 = NULL;

	ft_memcpy(str2, str, -1);
	printf("%s\n", str2);
	return 0;
} */
