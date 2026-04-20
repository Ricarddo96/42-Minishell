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

static void	add_var_to_env(t_sh *mini, char *arg)
{
	char	*new_var;

	new_var = ft_strdup(arg);
	if (!new_var)
	{
		free_matrix(mini->envp);
		free_cmd_list(mini->cmd_list);
		exit (1);
	}
	new_env_var(mini, new_var);
}

static void	update_env(t_sh *mini, char *arg, int j)
{
	char	*str;

	str = ft_strdup(arg);
	if (!str)
	{
		free_matrix(mini->envp);
		free_cmd_list(mini->cmd_list);
		exit (1);
	}
	free(mini->envp[j]);
	mini->envp[j] = str;
}

static int	is_in_env(t_sh *mini, char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[len] != '=' && str[len] != '\0')
		len++;
	while (mini->envp[i])
	{
		if (ft_strncmp(mini->envp[i], str, len) == 0)
		{
			if (mini->envp[i][len] == '\0' || mini->envp[i][len] == '=')
				return (i);
		}
		i++;
	}
	return (-1);
}

static int	export_check_args(t_sh *mini, char *str)
{
	int	i;

	i = 1;
	if (!ft_isalpha(str[0]) && str[0] != '_')
	{
		print_export_error_msg(str);
		mini->exit_status = 1;
		return (0);
	}
	while (str[i])
	{
		if (str[i] == '=')
			break ;
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			print_export_error_msg(str);
			mini->exit_status = 1;
			return (0);
		}
		i++;
	}
	return (1);
}

void	exec_export_args(t_sh *mini)
{
	int		i;
	int		j;

	i = 1;
	mini->exit_status = 0;
	while (mini->cmd_list->args[i])
	{
		if (export_check_args(mini, mini->cmd_list->args[i]))
		{
			j = is_in_env(mini, mini->cmd_list->args[i]);
			if (j != -1)
			{
				if (ft_strchr(mini->cmd_list->args[i], '='))
				{
					update_env(mini, mini->cmd_list->args[i], j);
				}
			}
			else
			{
				add_var_to_env(mini, mini->cmd_list->args[i]);
			}
		}
		i++;
	}
}
