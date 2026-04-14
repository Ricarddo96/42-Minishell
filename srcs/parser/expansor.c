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

static char	*append_var(char *result, char *token, int *i, t_sh *mini)
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

static int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

static void	update_qstate(char c, t_quote_state *qstate)
{
	if (c == '\'' && *qstate == NONE)
		*qstate = SINGLE;
	else if (c == '\'' && *qstate == SINGLE)
		*qstate = NONE;
	else if (c == '"' && *qstate == NONE)
		*qstate = DOUBLE;
	else if (c == '"' && *qstate == DOUBLE)
		*qstate = NONE;
}

char	*expand_token(char *token, t_sh *mini)
{
	char	*result;
	int		i;
	int		start;
	t_quote_state qstate;

	result = ft_strdup("");
	i = 0;
	qstate = NONE;
	while (token[i])
	{
		start = i;
		while (token[i] && !(token[i] == '$' && qstate != SINGLE)
			&& !is_quote_char(token[i]))
			i++;
		if (i > start)
			result = append_literal(result, token, start, i);
		if (token[i] == '\'' || token[i] == '"')
			update_qstate(token[i++], &qstate);
		else if (token[i] == '$' && qstate != SINGLE)
			result = append_var(result, token, &i, mini);
		if (!result)
			return (NULL);
	}
	return (result);
}

void	expand(t_tkn *seq, t_sh *mini)
{
	char	*expanded;

	while (seq)
	{
		if (seq->type == WORD)
		{
			expanded = expand_token(seq->token, mini);
			if (expanded)
			{
				free(seq->token);
				seq->token = expanded;
			}
		}
		seq = seq->next;
	}
}