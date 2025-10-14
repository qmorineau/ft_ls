/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:38:30 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 11:44:27 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	size_t	length;

	length = ft_strlen((char *) s1);
	ptr = malloc(sizeof(char) * length + 1);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s1, length + 1);
	return (ptr);
}
