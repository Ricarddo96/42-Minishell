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

int	is_built_in(char *cmd)
{
	if (!ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "pwd", 4)
		|| !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6)
		|| !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}

void    which_built_ins(t_sh *mini)
{
    if (!ft_strncmp(mini->cmd_list->args[0], "echo", 5))
		exec_echo(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "cd", 3))
		which_dir(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "pwd", 4))
		exec_pwd(mini);
	if (!ft_strncmp(mini->cmd_list->args[0], "export", 7))
		which_export(mini);
	if (!ft_strncmp(mini->cmd_list->args[0], "unset", 6))
		exec_unset(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "env", 4))
		exec_env(mini);
    if (!ft_strncmp(mini->cmd_list->args[0], "exit", 5))
		which_exit(mini);
}
