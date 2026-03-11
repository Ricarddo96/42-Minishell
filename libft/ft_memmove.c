/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:12:26 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 17:13:50 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*ptr_src;
	char	*ptr_dest;

	if ((!dest && !src) || (dest == src))
		return (dest);
	ptr_src = (char *)src;
	ptr_dest = (char *)dest;
	if (src < dest)
	{
		ptr_dest = ptr_dest + n;
		ptr_src = ptr_src + n;
		while (n--)
			*--ptr_dest = *--ptr_src;
	}
	else
	{
		while (n--)
			*ptr_dest++ = *ptr_src++;
	}
	return (dest);
}
