/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:24:26 by ridoming          #+#    #+#             */
/*   Updated: 2025/05/07 14:15:12 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 
#include <stdio.h>

char	*ft_strchr(const char *str, int c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == (unsigned char)c)
		{
			return ((char *)str + i);
		}
		i++;
	}
	if ((unsigned char)c == '\0')
	{
		return ((char *)str + i);
	}
	return (NULL);
}
