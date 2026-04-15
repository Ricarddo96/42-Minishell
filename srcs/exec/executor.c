/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:39:20 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/15 17:47:30 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
    int i;

    i = 0;
    
    return (path_plus_cmd);
}

char    *resolve_path(char *cmd, char **envp)
{
    char *cmd_route;
    
    cmd_route = path_join_cmd(get_path(envp), cmd);
    return (cmd_route);   
}

int executor(t_sh *mini)
{
    return (1);
}