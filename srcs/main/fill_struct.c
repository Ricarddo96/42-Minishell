/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:41:52 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/15 17:04:57 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	env_to_struct(t_sh *mini, char **envp)
{
	int	i;
	int	j;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
		{
			mini->envp[i][j] = envp[i][j];
			j++;
		}
		i++;
	}
}

static void	copy_env(t_sh *mini, char **envp)
{
	int	i;
	int	j;

	i = 0;
	while (envp[i])
		i++;
	mini->envp = ft_calloc(i + 1, sizeof(char *));
	if (!mini->envp)
		exit(1);
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
			j++;
		mini->envp[i] = ft_calloc(j + 1, sizeof(char));
		if (!mini->envp[i])
		{
			free_matrix(mini->envp);
			free_cmd_list(mini->cmd_list);
			exit (1);
		}
		i++;
	}
	env_to_struct(mini, envp);
}

void	fill_struct(t_sh *mini, char **envp)
{
	mini->exit_status = 0;
	mini->tkn_list = NULL;
	mini->cmd_list = NULL;
	copy_env(mini, envp);
}
