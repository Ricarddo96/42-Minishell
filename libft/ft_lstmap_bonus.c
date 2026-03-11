/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:47:08 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/21 13:21:12 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*node;
	void	*new_content;

	if (!lst || !f || !del)
	{
		return (NULL);
	}
	new_list = NULL;
	while (lst)
	{
		new_content = f(lst->content);
		node = ft_lstnew(new_content);
		if (!node)
		{
			if (new_content)
				del(new_content);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, node);
		lst = lst->next;
	}
	return (new_list);
}
