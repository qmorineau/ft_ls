/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:46:58 by qmorinea          #+#    #+#             */
/*   Updated: 2025/09/10 10:22:28 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	printf_putchar(char c, int *count)
{
	int	i;

	i = write(1, &c, 1);
	if (i < 0 || (*count) == -1)
		(*count) = -1;
	else
		(*count)++;
}

void	printf_putstr(char *s, int *count)
{
	size_t	i;

	i = 0;
	if (!s)
	{
		printf_putstr("(null)", count);
		return ;
	}
	while (s[i])
	{
		printf_putchar(s[i++], count);
		if (*count == -1)
			return ;
	}
}
