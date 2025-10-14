/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmorinea <qmorinea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:37:05 by qmorinea          #+#    #+#             */
/*   Updated: 2024/10/14 17:54:47 by qmorinea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_memmove_normal(unsigned char *ptr_dest, unsigned char *ptr_src,
	size_t n)
{
	while (n > 0)
	{
		*ptr_dest = *ptr_src;
		ptr_dest++;
		ptr_src++;
		n--;
	}
}

static void	ft_memmove_reverse(unsigned char *ptr_dest, unsigned char *ptr_src,
	size_t n)
{
	ptr_dest += n - 1;
	ptr_src += n - 1;
	while (n > 0)
	{
		*ptr_dest = *ptr_src;
		ptr_dest--;
		ptr_src--;
		n--;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*ptr_dest;
	unsigned const char	*ptr_src;

	if ((!dst && !src) || len == 0)
		return (dst);
	ptr_src = (unsigned const char *) src;
	ptr_dest = (unsigned char *) dst;
	if (ptr_dest < ptr_src || ptr_dest >= ptr_src + len)
	{
		ft_memmove_normal(ptr_dest, (unsigned char *) ptr_src, len);
	}
	else
	{
		ft_memmove_reverse(ptr_dest, (unsigned char *) ptr_src, len);
	}
	return (dst);
}
