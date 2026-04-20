/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:00:00 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/20 17:00:00 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**get_path(char **envp)
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

static char	*path_join_cmd(char **path, char *cmd)
{
	char	*path_plus_cmd;
	char	*cmd_slash;
	int		i;

	i = 0;
	cmd_slash = ft_strjoin("/", cmd);
	while (path[i])
	{
		path_plus_cmd = ft_strjoin(path[i], cmd_slash);
		if (!path_plus_cmd)
			break ;
		if (access(path_plus_cmd, X_OK) == 0)
			break ;
		free(path_plus_cmd);
		i++;
	}
	free(cmd_slash);
	if (!path[i])
		return (NULL);
	return (path_plus_cmd);
}

char	*resolve_path(char *cmd, char **envp)
{
	char	*cmd_route;
	char	**path;
	int		i;

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
