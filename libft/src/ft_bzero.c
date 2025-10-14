/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:35:32 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:55:19 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *) s;
	while (n > 0)
	{
		*ptr = '\0';
		ptr++;
		n--;
	}
}

/* 
#include <stdio.h>

int main(void)
{
    int i = 0;
    char c[13] = "abcdefghijkl";
    printf("%s\n", c);
    ft_bzero(c, 3);
    while (i < 13)
    {
        printf("%c", c[i]);
        i++;
    }
} */