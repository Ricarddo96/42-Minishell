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

// Está sin terminar
void	exec_unset(t_sh *mini)
{
	int	i;
	int	j;

	i = 1;
	if (mini->cmd_list->args[1] == NULL)
	{
		mini->exit_status = 0;
		return ;
	}
	while (mini->cmd_list->args[i])
	{
		j = 0;
		while (mini->envp[j])
		{
			if (ft_strnstr(mini->envp[j], mini->cmd_list->args[i], ft_strlen(mini->envp[j])))
			{
				free_var_in_env(mini, j);
				break ;
			}
			j++;
		}
		i++;
	}
	mini->exit_status = 0;
}

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
