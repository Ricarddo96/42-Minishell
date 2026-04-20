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

static int	unset_checker(char *envp, char *arg, int len)
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

static int	unset_check_args(t_sh *mini, int i)
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

static void	free_var_in_env(t_sh *mini, int j, int k)
{
	char	**new_env;
	int		i;

	i = 0;
	while (mini->envp[i])
		i++;
	new_env = calloc(sizeof(char *), i);
	if (!new_env)
		exit(1);
	i = 0;
	while (mini->envp[i])
	{
		if (i == j)
			i++;
		else
		{
			new_env[k] = mini->envp[i];
			i++;
			k++;
		}
	}
	free(mini->envp[j]);
	free(mini->envp);
	mini->envp = new_env;
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
					free_var_in_env(mini, j, 0);
					break ;
				}
				j++;
			}
		}
		i++;
	}
}
