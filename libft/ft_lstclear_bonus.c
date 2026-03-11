/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:34:25 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 14:35:09 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*copy;

	if (!lst || !del)
	{
		return ;
	}
	while (*lst)
	{
		copy = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = copy;
	}
}
