/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:38:27 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:58:39 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t		i;
	char		*p;

	i = 0;
	p = NULL;
	while (s[i])
	{
		if ((unsigned char) c == s[i])
		{
			p = (char *) &s[i];
			return (p);
		}
		i++;
	}
	if ((unsigned char) c == s[i])
		p = (char *) &s[i];
	return (p);
}
/* 
#include <stdio.h>

int main (void)
{
	char s[] = "abcdefg";
	printf("%s\n", strchr(s, 't' + 256));
} */