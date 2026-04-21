/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 12:40:00 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/21 16:42:17 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_pipes(int (*pipes)[2], int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static void	child_exec(t_cmd *cmd, t_sh *mini)
{
	char	*path;

	path = resolve_path(cmd->args[0], mini->envp);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	execve(path, cmd->args, mini->envp);
	perror("minishell");
	free(path);
	exit(126);
}

static void	handle_pipe_child(t_cmd *cmd, t_sh *mini, int (*pipes)[2], int i)
{
	int	n;

	handle_signals_child();
	n = count_cmds(mini->cmd_list);
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < n - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_pipes(pipes, n - 1);
	if (apply_redirs(cmd->redirs) < 0)
		exit(1);
	if (is_built_in(cmd->args[0]))
	{
		mini->cmd_list = cmd;
		which_built_ins(mini);
		exit(mini->exit_status);
	}
	child_exec(cmd, mini);
}

void	spawn_children(t_sh *mini, int (*pipes)[2], pid_t *pids, int n)
{
	t_cmd	*cmd;
	int		i;

	cmd = mini->cmd_list;
	i = 0;
	while (i < n)
		pids[i++] = -1;
	i = 0;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return ((void)perror("minishell: fork"));
		if (pids[i] == 0)
		{
			handle_pipe_child(cmd, mini, pipes, i);
			exit(1);
		}
		cmd = cmd->next;
		i++;
	}
}
