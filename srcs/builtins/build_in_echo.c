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

void	exec_echo(t_sh *mini)
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
    mini->exit_status = 0;
}
