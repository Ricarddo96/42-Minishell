/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:00:00 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/20 17:00:00 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_cmds(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

static void	close_pipes(int (*pipes)[2], int count)
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

int	exec_pipeline(t_sh *mini)
{
	int		(*pipes)[2];
	pid_t	*pids;
	t_cmd	*cmd;
	int		n;
	int		i;
	int		st;
	int		last_status;

	cmd = mini->cmd_list;
	n = count_cmds(mini->cmd_list);
	pipes = malloc(sizeof(int [2]) * (n - 1));
	if (!pipes)
		return (0);
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		return (free(pipes), 0);
	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			close_pipes(pipes, i);
			free(pipes);
			free(pids);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("minishell: fork");
			break ;
		}
		if (pids[i] == 0)
		{
			handle_pipe_child(cmd, mini, pipes, i);
			exit(1);
		}
		cmd = cmd->next;
		i++;
	}
	close_pipes(pipes, n - 1);
	last_status = 0;
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &st, 0);
		if (i == n - 1)
			last_status = st;
		i++;
	}
	if (WIFEXITED(last_status))
		mini->exit_status = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
		mini->exit_status = 128 + WTERMSIG(last_status);
	free(pipes);
	free(pids);
	return (mini->exit_status);
}
