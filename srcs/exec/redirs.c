/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:00:00 by ridoming          #+#    #+#             */
/*   Updated: 2026/05/17 13:17:29 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <unistd.h>

static void	heredoc_loop(int write_fd, char *delimiter, int len)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strlen(line) == len
			&& ft_strncmp(line, delimiter, len + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, write_fd);
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
}

int	read_heredoc(char *delimiter)
{
	int	fd[2];

	if (pipe(fd) == -1)
		return (-1);
	heredoc_loop(fd[1], delimiter, ft_strlen(delimiter));
	close(fd[1]);
	if (g_signal == 130)
	{
		close(fd[0]);
		return (-1);
	}
	return (fd[0]);
}

static int	read_cmd_heredocs(t_redir *r)
{
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			r->heredoc_fd = read_heredoc(r->file);
			if (r->heredoc_fd == -1)
				return (-1);
		}
		r = r->next;
	}
	return (0);
}

int	pre_read_heredocs(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	int		saved_stdin;
	int		status;

	saved_stdin = dup(STDIN_FILENO);
	handle_signals_heredoc();
	status = 0;
	cmd = cmd_list;
	while (cmd && status == 0)
	{
		status = read_cmd_heredocs(cmd->redirs);
		cmd = cmd->next;
	}
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	handle_signals_interactive();
	return (status);
}

int	apply_redirs(t_redir *redirs)
{
	int	fd;

	while (redirs)
	{
		if (redirs->type == REDIR_IN)
			fd = open(redirs->file, O_RDONLY);
		else if (redirs->type == REDIR_OUT)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		else if (redirs->type == REDIR_APPEND)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0664);
		else
			fd = redirs->heredoc_fd;
		if (fd == -1)
			return (perror("minishell"), -1);
		if (redirs->type == REDIR_IN || redirs->type == REDIR_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}
