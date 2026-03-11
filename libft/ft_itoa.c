/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:26:21 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 14:30:54 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_str(long long l, int sign, int d, char *s)
{
	s[d + sign] = '\0';
	if (l == 0)
	{
		s[0] = '0';
	}
	else
	{
		while (d > 0)
		{
			s[d + sign - 1] = (l % 10) + '0';
			l /= 10;
			d--;
		}
		if (sign)
			s[0] = '-';
	}
}

char	*ft_itoa(int n)
{
	long long	n_long;
	int			sign;
	int			digits;
	long long	temp;
	char		*str;

	n_long = n;
	digits = 1;
	sign = (n_long < 0);
	if (sign)
	{
		n_long = -n_long;
	}
	temp = n_long;
	while (temp >= 10)
	{
		temp /= 10;
		digits++;
	}
	str = malloc(digits + sign + 1);
	if (!str)
		return (NULL);
	fill_str(n_long, sign, digits, str);
	return (str);
}
