/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 12:57:02 by ridoming          #+#    #+#             */
/*   Updated: 2026/03/18 14:16:15 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	error_msg(char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (0);
}

static int	validate_pipe_sintax(t_tkn *seq)
{
	if (!seq->prev || !seq->next)
		return (error_msg("syntax error near unexpected token `|'"));
	if (seq->next->type == PIPE)
		return (error_msg("syntax error near unexpected token `|'"));
	return (1);
}

static int	validate_redir_sintax(t_tkn *seq)
{
	if (!seq->next || seq->next->type != WORD)
		return (error_msg("syntax error near unexpected token `newline'"));
	return (1);
}

int	validate(t_tkn *seq)
{
	t_tkn	*seq_copy;

	if (!seq)
		return (0);
	seq_copy = seq;
	while (seq_copy)
	{
		if (seq_copy->type == PIPE)
		{
			if (!validate_pipe_sintax(seq_copy))
				return (0);
		}
		else if (seq_copy->type == REDIR_APPEND || seq_copy->type == REDIR_OUT
			|| seq_copy->type == REDIR_HEREDOC || seq_copy->type == REDIR_IN)
		{
			if (!validate_redir_sintax(seq_copy))
				return (0);
		}
		seq_copy = seq_copy->next;
	}
	return (1);
}