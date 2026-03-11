/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:55:34 by ridoming          #+#    #+#             */
/*   Updated: 2025/07/24 18:24:08 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	ft_atof(const char *nptr)
{
	int		i;
	double	posicion;
	double	signo;
	double	resultado;

	i = 0;
	signo = 1.0;
	resultado = 0.0;
	posicion = 1.0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-')
		signo = -1.0;
	if (nptr[i] == '-' || nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9' && nptr[i] != '.')
		resultado = resultado * 10 + (nptr[i++] - '0');
	i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		posicion *= 10;
		resultado = resultado + ((nptr[i++] - '0') / posicion);
	}
	return (signo * resultado);
}
