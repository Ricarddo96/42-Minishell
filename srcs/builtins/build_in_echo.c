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

static int	check_n_flag(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-' || str[1] == '\0')
		return (0);
	while (str[i] == 'n')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	exec_echo(t_sh *mini)
{
	int	i;
	int	has_line_break;

	mini->exit_status = 0;
	if (!mini->cmd_list->args[1])
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		return ;
	}
	has_line_break = 1;
	i = 1;
	while (mini->cmd_list->args[i] && check_n_flag(mini->cmd_list->args[i]))
	{
		has_line_break = 0;
		i++;
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
