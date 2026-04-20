/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:00:00 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/20 17:00:00 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_fork_one_cmd(t_sh *mini, char *path)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell");
		free(path);
		return ;
	}
	if (pid == 0)
	{
		if (apply_redirs(mini->cmd_list->redirs) == -1)
			exit(1);
		execve(path, mini->cmd_list->args, mini->envp);
		perror("minishell");
		free(path);
		exit(1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		mini->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		mini->exit_status = 128 + WTERMSIG(status);
	free(path);
}

void	exec_one_cmd(t_sh *mini)
{
	char	*path;

	path = resolve_path(mini->cmd_list->args[0], mini->envp);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(mini->cmd_list->args[0], 2);
		ft_putstr_fd("\n", 2);
		mini->exit_status = 127;
		return ;
	}
	handle_fork_one_cmd(mini, path);
}
