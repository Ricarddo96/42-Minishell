/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:00:00 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/21 16:45:17 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_cmds(t_cmd *cmd)
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

int	create_pipes(int (*pipes)[2], int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("minishell: pipe");
			close_pipes(pipes, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	wait_children(pid_t *pids, int n)
{
	int	i;
	int	st;
	int	last;

	last = 0;
	st = 0;
	i = 0;
	while (i < n)
	{
		if (pids[i] != -1)
		{
			waitpid(pids[i], &st, 0);
			if (i == n - 1)
				last = st;
		}
		i++;
	}
	return (last);
}

static void	set_exit_status(t_sh *mini, int last_status, int last_ok)
{
	if (!last_ok)
		mini->exit_status = 1;
	else if (WIFEXITED(last_status))
		mini->exit_status = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status))
	{
		if (WTERMSIG(last_status) == SIGINT)
			write(STDERR_FILENO, "\n", 1);
		mini->exit_status = 128 + WTERMSIG(last_status);
	}
}

int	exec_pipeline(t_sh *mini)
{
	int		(*pipes)[2];
	pid_t	*pids;
	int		n;
	int		last;

	n = count_cmds(mini->cmd_list);
	pipes = malloc(sizeof(int [2]) * (n - 1));
	pids = malloc(sizeof(pid_t) * n);
	if (!pipes || !pids)
		return (free(pipes), free(pids), 0);
	if (create_pipes(pipes, n) < 0)
	{
		mini->exit_status = 1;
		return (free(pipes), free(pids), mini->exit_status);
	}
	spawn_children(mini, pipes, pids, n);
	close_pipes(pipes, n - 1);
	handle_signals_exec();
	last = wait_children(pids, n);
	handle_signals_interactive();
	set_exit_status(mini, last, pids[n - 1] != -1);
	free(pipes);
	free(pids);
	return (mini->exit_status);
}
