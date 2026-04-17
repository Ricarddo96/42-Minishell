/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:58:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/14 18:49:45 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_msg(char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (0);
}

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

void	copy_env(t_sh *mini, char **envp)
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

static int	check_unclosed_quotes(char *line)
{
	int				i;
	t_quote_state	qstate;

	i = 0;
	qstate = NONE;
	while (line[i])
	{
		if (line[i] == '\'' && qstate == NONE)
			qstate = SINGLE;
		else if (line[i] == '\'' && qstate == SINGLE)
			qstate = NONE;
		else if (line[i] == '"' && qstate == NONE)
			qstate = DOUBLE;
		else if (line[i] == '"' && qstate == DOUBLE)
			qstate = NONE;
		i++;
	}
	if (qstate != NONE)
		return (error_msg("syntax error: unclosed quotes"));
	return (1);
}

int	parser(char *line, t_sh *mini)
{
	if (!check_unclosed_quotes(line))
	{
		mini->exit_status = 2;
		return (0);
	}
	mini->tkn_list = tokenize(line, mini);
	if (!mini->tkn_list)
		return (0);
	if (!validate(mini->tkn_list, mini))
	{
		free_tokens(mini->tkn_list);
		mini->tkn_list = NULL;
		return (0);
	}
	expand(mini->tkn_list, mini);
	build_cmd_list(mini->tkn_list, &mini->cmd_list);
	free_tokens(mini->tkn_list);
	mini->tkn_list = NULL;
	return (1);
}
