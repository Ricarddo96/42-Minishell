/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:13:33 by ridoming          #+#    #+#             */
/*   Updated: 2026/03/19 16:52:47 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static char	*get_var_value(char *name, char **envp)
{
	int	name_len;
	int	j;

	name_len = ft_strlen(name);
	j = 0;
	while (envp[j])
	{
		if (ft_strncmp(envp[j], name, name_len) == 0
			&& envp[j][name_len] == '=')
			return (envp[j] + name_len + 1);
		j++;
	}
	return ("");
}

static char	*append_literal(char *result, char *token, int start, int end)
{
	char	*segment;
	char	*tmp;

	segment = ft_substr(token, start, end - start);
	tmp = ft_strjoin(result, segment);
	free(segment);
	free(result);
	return (tmp);
}

static char	*append_var(char *result, char *token, int *i, char **envp)
{
	char	*name;
	char	*tmp;

	(*i)++;
	name = get_var_name(token, i);
	if (!name)
	{
		free(result);
		return (NULL);
	}
	tmp = ft_strjoin(result, get_var_value(name, envp));
	free(name);
	free(result);
	return (tmp);
}

char	*expand_token(char *token, char **envp)
{
	char	*result;
	int		i;
	int		start;

	result = ft_strdup("");
	i = 0;
	while (token[i])
	{
		start = i;
		while (token[i] && token[i] != '$')
			i++;
		if (i > start)
			result = append_literal(result, token, start, i);
		if (token[i] == '$')
			result = append_var(result, token, &i, envp);
		if (!result)
			return (NULL);
	}
	return (result);
}

t_tkn	*env_expand(t_tkn *seq, char **envp)
{
	t_tkn	*head;
	char	*expanded;

	head = seq;
	while(seq)
	{
		if (!seq->single_quoted && ft_strchr(seq->token, '$'))
		{
			expanded = expand_token(seq->token, envp);
			if (!expanded)
				return (NULL);
			free(seq->token);
			seq->token = expanded;
		}
		seq = seq->next;
	}
	return (head);
}