/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:40:13 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 18:46:21 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <ctype.h>

int	ft_toupper(int c)
{
	unsigned char	d;

	if (c == EOF)
	{
		return (EOF);
	}
	d = (unsigned char)c;
	if (d >= 'a' && d <= 'z')
	{
		return (c - 32);
	}
	return (c);
}
