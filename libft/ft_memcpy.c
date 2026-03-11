/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:09:41 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 17:11:39 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned int	i;
	unsigned char	*ptr_dest;
	unsigned char	*ptr_src;

	i = 0;
	if ((dest == NULL && src == NULL) || (dest == src))
	{
		return (dest);
	}
	ptr_src = (unsigned char *)src;
	ptr_dest = (unsigned char *)dest;
	while (i < n)
	{
		ptr_dest[i] = ptr_src[i];
		i++;
	}
	return (dest);
}
