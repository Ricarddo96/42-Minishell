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

static int	check_if_append(char c, t_quote_state qstate)
{
	if (c && ((c != '$' || qstate == SINGLE) && (c != '"' && c != '\'')))
		return (1);
	else if ((c == '\'' && qstate == DOUBLE) || (c == '"' && qstate == SINGLE))
		return (1);
	else
		return (0);
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

static char	*expand_token(char *token, t_sh *mini)
{
	char			*result;
	int				i;
	int				start;
	t_quote_state	qstate;

	result = ft_strdup("");
	i = 0;
	qstate = NONE;
	while (token[i])
	{
		start = i;
		while (check_if_append(token[i], qstate))
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
