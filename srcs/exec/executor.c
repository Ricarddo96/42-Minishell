/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:39:20 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/21 12:59:48 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	exec_single_builtin(t_sh *mini)
{
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in < 0 || saved_out < 0)
	{
		mini->exit_status = 1;
		return ;
	}
	if (apply_redirs(mini->cmd_list->redirs) == 0)
		which_built_ins(mini);
	else
		mini->exit_status = 1;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

static void	exec_redirs_only(t_sh *mini)
{
	int	saved_in;
	int	saved_out;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in < 0 || saved_out < 0)
	{
		mini->exit_status = 1;
		return ;
	}
	if (apply_redirs(mini->cmd_list->redirs) == 0)
		mini->exit_status = 0;
	else
		mini->exit_status = 1;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
}

int	executor(t_sh *mini)
{
	if (!mini->cmd_list)
		return (mini->exit_status);
	if (!mini->cmd_list->args)
	{
		if (mini->cmd_list->redirs)
			exec_redirs_only(mini);
		free_cmd_list(mini->cmd_list);
		mini->cmd_list = NULL;
		return (mini->exit_status);
	}
	if (mini->cmd_list->next == NULL)
	{
		if (is_built_in(mini->cmd_list->args[0]))
			exec_single_builtin(mini);
		else
			exec_one_cmd(mini);
	}
	else
		exec_pipeline(mini);
	free_cmd_list(mini->cmd_list);
	mini->cmd_list = NULL;
	return (mini->exit_status);
}
