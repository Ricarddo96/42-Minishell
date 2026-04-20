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

static void	exec_std_exit(t_sh *mini)
{
	free_cmd_list(mini->cmd_list);
	free_matrix(mini->envp);
	exit(mini->exit_status);
}

static void	exec_char_exit(t_sh *mini)
{
	ft_putstr_fd("bash: exit: ", STDERR_FILENO);
	ft_putstr_fd(mini->cmd_list->args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	free_cmd_list(mini->cmd_list);
	free_matrix(mini->envp);
	exit (255);
}

static int	is_num(char *arg)
{
	int	i;

	i = 0;
	if (!arg[i])
		return (0);
	if (arg[i] == '-' || arg[i] == '+')
	{
		i++;
		if (!arg[i])
			return (0);
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exec_num_exit(t_sh *mini)
{
	long long	exit_num;
	int			error;

	error = 1;
	exit_num = ft_atoi_buildins(mini->cmd_list->args[1], &error);
	if (!error)
		exec_char_exit(mini);
	free_cmd_list(mini->cmd_list);
	free_matrix(mini->envp);
	exit ((unsigned char)exit_num);
}

void	which_exit(t_sh *mini)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (mini->cmd_list->args[1] == NULL)
		exec_std_exit(mini);
	else if (!is_num(mini->cmd_list->args[1]))
		exec_char_exit(mini);
	else if (mini->cmd_list->args[2] != NULL)
		too_many_args(mini);
	else
		exec_num_exit(mini);
}
