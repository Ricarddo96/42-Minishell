/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 12:57:02 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/14 13:22:03 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	validate_redir_sintax(t_tkn *seq)
{
	if (!seq->next)
		return (error_msg("syntax error near unexpected token `newline'"));
	if (seq->next->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(seq->next->token, 2);
		ft_putstr_fd("'\n", 2);
		return (0);
	}
	return (1);
}

static int	validate_pipe_sintax(t_tkn *seq)
{
	if (!seq->prev || !seq->next)
		return (error_msg("syntax error near unexpected token `|'"));
	if (seq->next->type == PIPE)
		return (error_msg("syntax error near unexpected token `|'"));
	return (1);
}

int	validate(t_tkn *seq, t_sh *mini)
{
	t_tkn	*seq_copy;

	if (!seq)
		return (1);
	seq_copy = seq;
	while (seq_copy)
	{
		if (seq_copy->type == PIPE && !validate_pipe_sintax(seq_copy))
		{
			mini->exit_status = 2;
			return (0);
		}
		else if (seq_copy->type == REDIR_APPEND || seq_copy->type == REDIR_OUT
			|| seq_copy->type == REDIR_HEREDOC || seq_copy->type == REDIR_IN)
		{
			if (!validate_redir_sintax(seq_copy))
			{
				mini->exit_status = 2;
				return (0);
			}
		}
		seq_copy = seq_copy->next;
	}
	return (1);
}
