/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:23:36 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/25 19:45:37 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*memoria;

	if (nmemb == 0 || size == 0)
	{
		memoria = malloc(1);
		ft_bzero(memoria, 1);
		return (memoria);
	}
	if (nmemb > __SIZE_MAX__ / size)
	{
		return (NULL);
	}
	memoria = (char *)malloc(nmemb * size);
	if (memoria == NULL)
	{
		return (NULL);
	}
	ft_memset(memoria, 0, nmemb * size);
	return (memoria);
}
