/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:44:23 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/22 16:15:44 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	dest_long;
	size_t	dest_long_copy;
	size_t	src_long;

	i = 0;
	dest_long = ft_strlen(dest);
	src_long = ft_strlen(src);
	dest_long_copy = dest_long;
	if (size <= dest_long)
		return (size + src_long);
	if (size != 0)
	{
		while (i < size - dest_long - 1 && src[i] != '\0')
		{
			dest[dest_long_copy] = src[i];
			dest_long_copy++;
			i++;
		}
		if (dest_long_copy < size)
			dest[dest_long_copy] = '\0';
	}
	return (dest_long + src_long);
}
