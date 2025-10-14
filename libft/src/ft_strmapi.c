/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:38:47 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/09 10:38:47 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*string;

	if (!s || !(*f))
		return (NULL);
	string = malloc(sizeof(char) * (ft_strlen((char *)s) + 1));
	if (!string)
		return (NULL);
	i = 0;
	while (s[i])
	{
		string[i] = (*f)(i, s[i]);
		i++;
	}
	string[i] = '\0';
	return (string);
}

/* #include <stdio.h>

int main (void)
{
	char s[] = "abc";
	printf("%s", ft_strmapi(s, NULL));
} */