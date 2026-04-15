/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:13:33 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/14 13:45:16 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static char	*get_var_name(char *token, int *i)
{
	int	start;

	if (token[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	start = *i;
	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_'))
		(*i)++;
	return (ft_substr(token, start, *i - start));
}

static char	*get_var_value(char *name, t_sh *mini)
{
	int	name_len;
	int	j;

	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(mini->exit_status));
	name_len = ft_strlen(name);
	j = 0;
	while (mini->envp[j])
	{
		if (ft_strncmp(mini->envp[j], name, name_len) == 0
			&& mini->envp[j][name_len] == '=')
			return (ft_strdup(mini->envp[j] + name_len + 1));
		j++;
	}
	return (ft_strdup(""));
}

char	*append_var(char *result, char *token, int *i, t_sh *mini)
{
	char	*name;
	char	*value;
	char	*tmp;

	(*i)++;
	if (!token[*i] || (!ft_isalnum(token[*i]) && token[*i] != '_'
			&& token[*i] != '?'))
	{
		tmp = ft_strjoin(result, "$");
		free(result);
		return (tmp);
	}
	name = get_var_name(token, i);
	if (!name)
	{
		free(result);
		return (NULL);
	}
	value = get_var_value(name, mini);
	free(name);
	tmp = ft_strjoin(result, value);
	free(value);
	free(result);
	return (tmp);
}
