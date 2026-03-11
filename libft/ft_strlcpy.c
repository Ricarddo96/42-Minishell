/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:05:53 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 18:07:53 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t		i;
	size_t		src_long;
	const char	*src_copia;

	src_copia = src;
	i = 0;
	src_long = 0;
	while (*src_copia != '\0')
	{
		src_long++;
		src_copia++;
	}
	if (size != 0)
	{
		while (i < size - 1 && src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (src_long);
}
