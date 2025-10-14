/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:00:06 by qmorinea          #+#    #+#             */
/*   Updated: 2025/09/10 10:22:09 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	put_nbr_base_int(long long nbr, char *base, int *count)
{
	long	base_len;
	long	tmp;

	base_len = ft_strlen(base);
	if (*count == -1)
		return ;
	if (nbr < 0)
	{
		printf_putchar('-', count);
		nbr *= -1;
	}
	if (nbr < base_len)
	{
		printf_putchar(base[nbr], count);
	}
	else
	{
		tmp = nbr % base_len;
		put_nbr_base_int(nbr / base_len, base, count);
		if (*count == -1)
			return ;
		printf_putchar(base[tmp], count);
	}
}

static void	put_nbr_base_long(size_t nbr, char *base, int *count)
{
	size_t	base_len;
	size_t	tmp;

	base_len = ft_strlen(base);
	if (*count == -1)
		return ;
	if (nbr < base_len)
		printf_putchar(base[nbr], count);
	else
	{
		tmp = nbr % base_len;
		put_nbr_base_long(nbr / base_len, base, count);
		if (*count == -1)
			return ;
		printf_putchar(base[tmp], count);
	}
}

static void	put_nbr_base(long long nbr, char *base, int *count, int type)
{
	if (type == INT)
		put_nbr_base_int((int) nbr, base, count);
	else if (type == U_INT)
		put_nbr_base_long((unsigned int) nbr, base, count);
	else if (type == LONG)
		put_nbr_base_long((size_t) nbr, base, count);
	else
		return ;
}

static void	print_from_flags(va_list args, int *count, char c)
{
	if (c == 'd' || c == 'i')
		put_nbr_base(va_arg(args, long), BASE_TEN, count, INT);
	else if (c == 's')
		printf_putstr(va_arg(args, char *), count);
	else if (c == 'c')
		printf_putchar(va_arg(args, int), count);
	else if (c == '%')
		printf_putchar('%', count);
	else if (c == 'p')
	{
		printf_putstr("0x", count);
		put_nbr_base(va_arg(args, size_t), HEXA_LOW, count, LONG);
	}
	else if (c == 'x')
		put_nbr_base(va_arg(args, int), HEXA_LOW, count, U_INT);
	else if (c == 'X')
		put_nbr_base(va_arg(args, int), HEXA_UPP, count, U_INT);
	else if (c == 'u')
		put_nbr_base(va_arg(args, unsigned int), BASE_TEN, count, U_INT);
	else
		printf_putchar('%', count);
}

int	ft_printf(const char *string, ...)
{
	va_list	args;
	int		count;
	int		i;

	i = 0;
	count = 0;
	va_start(args, string);
	while (string[i])
	{
		if (string[i] != '%')
			printf_putchar(string[i++], &count);
		else
		{
			i++;
			print_from_flags(args, &count, string[i]);
			i++;
		}
		if (count == -1)
			return (-1);
	}
	va_end(args);
	return (count);
}
