/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:50:30 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 14:51:49 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	list_size;

	list_size = 0;
	while (lst != NULL)
	{
		lst = lst->next;
		list_size++;
	}
	return (list_size);
}
