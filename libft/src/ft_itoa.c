/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:36:22 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/10 12:35:30 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_abs_value(int n)
{
	if (n < 0)
		return (n * -1);
	return (n);
}

static int	ft_itoa_length(int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		i++;
		n *= -1;
	}
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		length;
	int		is_neg;

	length = ft_itoa_length(n);
	result = malloc(sizeof(char) * (length + 1));
	if (!result)
		return (NULL);
	is_neg = 0;
	if (n < 0)
		is_neg = 1;
	result[length] = '\0';
	length--;
	if (n == 0)
		result[length] = '0';
	while (n != 0)
	{
		result[length] = ft_abs_value(n % 10) + '0';
		n /= 10;
		length--;
	}
	if (is_neg == 1)
		result[0] = '-';
	return (result);
}
