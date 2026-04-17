/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 15:43:57 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/16 15:45:29 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	new_env_var(t_sh *mini, char *new_var)
{
	char	**new_env;
	int		i;

	i = 0;
	while (mini->envp[i])
		i++;
	new_env = calloc(i + 2, sizeof(char *));
	if (!new_env)
		exit (1);
	i = 0;
	while (mini->envp[i])
	{
		new_env[i] = mini->envp[i];
		i++;
	}
	new_env[i] = new_var;
	free(mini->envp);
	mini->envp = new_env;
}

static void	update_wd_in_env(t_sh *mini, char *env_var, char *new_pwd)
{
	int		i;
	char	*new_var;

	i = 0;
	new_var = ft_strjoin(env_var, new_pwd);
	while (mini->envp[i] != NULL)
	{	
		if (ft_strnstr(mini->envp[i], env_var, ft_strlen(env_var)))
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
	update_wd_in_env(mini, "OLDPWD=", wd);
	if (chdir(dir) != 0)
	{
		perror("cd: ");
		//error_msg( "cd: No such file or directory");
		mini->exit_status = 1;
		return ;
	}
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
		if (ft_strnstr(mini->envp[i], "HOME=", 5))
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

static void	which_dir(t_sh *mini)
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

static void	exec_echo(const t_sh *mini)
{
	int	i;
	int	has_line_break;

	if (!ft_strncmp(mini->cmd_list->args[1], "-n", 3))
	{
		i = 2;
		has_line_break = 0;
	}
	else
	{
		i = 1;
		has_line_break = 1;
	}
	while (mini->cmd_list->args[i])
		{
			ft_putstr_fd(mini->cmd_list->args[i], STDOUT_FILENO);
			i++;
			if (mini->cmd_list->args[i])
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
	if (has_line_break)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

void    which_built_ins(t_sh *mini)
{
    if (!ft_strncmp(mini->cmd_list->args[0], "echo", 5))
		exec_echo(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "cd", 3))
		which_dir(mini);
    /*if (!ft_strncmp(mini->cmd_list->args[0], "pwd", 4))
		exec_pwd(mini);
	if (!ft_strncmp(mini->cmd_list->args[0], "export", 7))
		exec_expor(mini);
	if (!ft_strncmp(mini->cmd_list->args[0], "unset", 6))
		exec_unset(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "env", 4))
		exec_env(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "exit", 5))
		exec_exit(mini);
*/}
