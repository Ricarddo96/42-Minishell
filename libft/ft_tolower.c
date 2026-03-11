/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:35:56 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 18:39:33 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ctype.h>

int	ft_tolower(int c)
{
	unsigned char	d;

	if (c == EOF)
	{
		return (EOF);
	}
	d = (unsigned char)c;
	if (d >= 'A' && d <= 'Z')
	{
		return (c + 32);
	}
	return (c);
}
