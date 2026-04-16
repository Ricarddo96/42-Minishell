/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:39:20 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/16 17:13:00 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <fcntl.h>
#include <unistd.h>


char	**get_path(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			if (!paths)
				ft_putstr_fd("Error finding path\n", 2);
			return (paths);
		}
		i++;
	}
	return (NULL);
}

char *path_join_cmd(char **path, char *cmd)
{
    char *path_plus_cmd;
	char *cmd_slash;
    int i;

    i = 0;
	cmd_slash = ft_strjoin("/", cmd);
    while (path[i]) 
	{
		path_plus_cmd = ft_strjoin(path[i], cmd_slash);
		if (access(path_plus_cmd, X_OK) == 0) 
			break;
		free(path_plus_cmd);
		i++;
	}
	free(cmd_slash);
	if (!path[i])
		return (NULL);
    return (path_plus_cmd);
}

char    *resolve_path(char *cmd, char **envp)
{
    char *cmd_route;
	char **path;
	int i;
    
	i = 0;
	if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    } 
	path = get_path(envp);
	if (!path)
		return (NULL);
	cmd_route = path_join_cmd(path, cmd);
	while (path[i]) 
		free(path[i++]);
	free(path);
    return (cmd_route);   
}

int apply_redirs(t_redir *redirs)
{
	int fd;

	while (redirs) 
	{
		if (redirs->type == REDIR_IN)
			fd = open(redirs->file, O_RDONLY);
		else if (redirs->type == REDIR_OUT)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
		else if (redirs->type == REDIR_APPEND)
			fd = open(redirs->file, O_WRONLY | O_CREAT | O_APPEND, 0664);
		else
		{
			redirs = redirs->next; // esto es para el heredoc que lo dejamos para mas adelante
			continue;
		} 
		if (fd == -1)
        {
            perror("minishell");
            return (-1);
        }
		if (redirs->type == REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}

void handle_fork_one_cmd(t_sh *mini, char *path)
{
	pid_t pid;
	int status;
	
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
	else 
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			mini->exit_status = WEXITSTATUS(status);
		free(path);
	}
}

void exec_one_cmd(t_sh *mini)
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

int executor(t_sh *mini)
{
    if (mini->cmd_list == NULL)
        return (0);
    if (mini->cmd_list->next == NULL) // comando simple (un solo comando)
    {
        if (is_built_in(mini->cmd_list->args[0]))
        {
            // ejecutar built-in sin fork
			return (mini->exit_status);
        }
        else
        {
            exec_one_cmd(mini);
			return (mini->exit_status);
        }
    }
    else
    {
		return (0);
        // pipeline: iterar cmd_list, crear pipes, fork por cada comando
    }
    return (mini->exit_status);
}