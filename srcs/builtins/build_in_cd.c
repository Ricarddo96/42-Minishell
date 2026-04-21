/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 15:43:57 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/21 12:56:52 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	update_wd_in_env(t_sh *mini, char *env_var, char *new_pwd)
{
	int		i;
	char	*new_var;

	i = 0;
	new_var = ft_strjoin(env_var, new_pwd);
	while (mini->envp[i] != NULL)
	{
		if (ft_strncmp(mini->envp[i], env_var, ft_strlen(env_var)) == 0)
		{
			free(mini->envp[i]);
			mini->envp[i] = new_var;
			return ;
		}
		i++;
	}
	new_env_var(mini, new_var);
}

static void	exec_cd(t_sh *mini, char *dir)
{
	char	wd[PATH_MAX];

	if (getcwd(wd, sizeof(wd)) == NULL)
	{
		error_msg("getcwd failed");
		mini->exit_status = 1;
		return ;
	}
	if (chdir(dir) != 0)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(dir);
		mini->exit_status = 1;
		return ;
	}
	update_wd_in_env(mini, "OLDPWD=", wd);
	if (getcwd(wd, sizeof(wd)) == NULL)
	{
		error_msg("getcwd failed");
		mini->exit_status = 1;
		return ;
	}
	update_wd_in_env(mini, "PWD=", wd);
	mini->exit_status = 0;
}

static void	search_cd_home(t_sh *mini)
{
	int		i;
	char	*dir;

	i = 0;
	while (mini->envp[i] != NULL)
	{
		if (ft_strncmp(mini->envp[i], "HOME=", 5) == 0)
			break ;
		i++;
	}
	if (mini->envp[i] == NULL)
	{
		error_msg("cd: HOME not set");
		mini->exit_status = 1;
		return ;
	}
	dir = ft_strdup(mini->envp[i] + 5);
	exec_cd(mini, dir);
	free(dir);
}

void	which_dir(t_sh *mini)
{
	if (mini->cmd_list->args[2] != NULL)
	{
		error_msg("cd: too many arguments");
		mini->exit_status = 1;
		return ;
	}
	if (mini->cmd_list->args[1] == NULL)
		search_cd_home(mini);
	else
		exec_cd(mini, mini->cmd_list->args[1]);
}
