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

void	exec_pwd(t_sh *mini)
{
	int		i;
	char	wd[PATH_MAX];

	i = 0;
	while (mini->envp[i])
	{
		if (ft_strnstr(mini->envp[i], "PWD=", 5))
			break ;
		i++;
	}
	if (mini->envp[i] == NULL)
	{
		if (getcwd(wd, sizeof(wd)) == NULL)
		{
			error_msg("getcwd failed");
			mini->exit_status = 1;
			return ;
		}
		ft_putendl_fd(wd, STDOUT_FILENO);
	}
	else
		ft_putendl_fd(mini->envp[i] + 4, STDOUT_FILENO);
	mini->exit_status = 0;
}
