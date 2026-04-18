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

void	exec_env(t_sh *mini)
{
	int	i;

	i = 0;
	if (mini->cmd_list->args[1] != NULL)
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd(mini->cmd_list->args[1], STDERR_FILENO);
		ft_putendl_fd("': no such file or directory", STDERR_FILENO);
		mini->exit_status = 127;
		return ;
	}
	while (mini->envp[i])
	{
		if (ft_strchr(mini->envp[i], '='))
			ft_putendl_fd(mini->envp[i], STDOUT_FILENO);
		i++;
	}
	mini->exit_status = 0;
}
