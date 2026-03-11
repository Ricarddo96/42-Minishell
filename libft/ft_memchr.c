/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:52:28 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 15:32:15 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	a;

	i = 0;
	str = (unsigned char *)s;
	a = (unsigned char)c;
	while (i < n)
	{
		if (str[i] == a)
		{
			return (str + i);
		}
		i++;
	}
	return (NULL);
}
