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
/*
int	unset_checker(char *envp, char *arg, int len)
{
	if (ft_strncmp(envp, arg, len) == 0)
	{
		if (envp[len] == '=' || envp[len] == '\0')
		{
			return (1);
		}
	}
	return (0);
}

void	print_unset_error(char *str)
{
	ft_putstr_fd("minishell: unset: '", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

int	unset_check_args(t_sh *mini, int i)
{
	int		j;
	char	*ptr;

	j = 1;
	ptr = mini->cmd_list->args[i];
	if (!ft_isalpha(ptr[0]) && ptr[0] != '_')
	{
		mini->exit_status = 1;
		print_unset_error(ptr);
		return (0);
	}
	while (ptr[j])
	{
		if (!ft_isalnum(ptr[j]) && ptr[j] != '_')
		{
			mini->exit_status = 1;
			print_unset_error(ptr);
			return (0);
		}
		j++;
	}
	return (1);
}

void	exec_unset(t_sh *mini)
{
	int	i;
	int	j;
	int	len;

	i = 1;
	mini->exit_status = 0;
	while (mini->cmd_list->args[i])
	{
		j = 0;
		if (unset_check_args(mini, i))
		{
			len = ft_strlen(mini->cmd_list->args[i]);
			while (mini->envp[j])
			{
				if (unset_checker(mini->envp[j], mini->cmd_list->args[i], len))
				{
					free_var_in_env(mini, j);
					break ;
				}
				j++;
			}
		}
		i++;
	}
}
*/
void    which_built_ins(t_sh *mini)
{
    if (!ft_strncmp(mini->cmd_list->args[0], "echo", 5))
		exec_echo(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "cd", 3))
		which_dir(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "pwd", 4))
		exec_pwd(mini);
	/*if (!ft_strncmp(mini->cmd_list->args[0], "export", 7))
		exec_expor(mini);
	if (!ft_strncmp(mini->cmd_list->args[0], "unset", 6))
		exec_unset(mini);
    */if (!ft_strncmp(mini->cmd_list->args[0], "env", 4))
		exec_env(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "exit", 5))
		which_exit(mini);
}
