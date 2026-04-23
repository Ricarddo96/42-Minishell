/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 12:57:02 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/23 15:58:14 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

void	free_tokens(t_tkn *list)
{
	t_tkn	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->token);
		free(list);
		list = tmp;
	}
}

void	free_redirs(t_redir *list)
{
	t_redir	*tmp;

	while (list)
	{
		tmp = list->next;
		if (list->heredoc_fd != -1)
			close(list->heredoc_fd);
		free(list->file);
		free(list);
		list = tmp;
	}
}

void	free_cmd_list(t_cmd *list)
{
	t_cmd	*tmp;
	int		i;

	while (list)
	{
		tmp = list->next;
		if (list->args)
		{
			i = 0;
			while (list->args[i])
				free(list->args[i++]);
			free(list->args);
		}
		free_redirs(list->redirs);
		free(list);
		list = tmp;
	}
}
