/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:05:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/14 16:16:39 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	tokenize_pipe(char *line, int i, t_tkn **tkn_list)
{
	t_tkn	*token;

	(void)line;
	token = new_token("|", PIPE);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 1);
}

static int	is_word_end(char c, t_quote_state qstate)
{
	if (qstate != NONE)
		return (0);
	return (c == ' ' || c == '\t' || c == '<' || c == '>'
		|| c == '|');
}

static void	update_quote_state(char c, t_quote_state *qstate)
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

static int	tokenize_complex_word(char *line, int i, t_tkn **tkn_list)
{
	int				start;
	char			*word;
	t_tkn			*token;
	t_quote_state	qstate;

	start = i;
	qstate = NONE;
	while (line[i] && !is_word_end(line[i], qstate))
	{
		update_quote_state(line[i], &qstate);
		i++;
	}
	word = ft_substr(line, start, i - start);
	if (!word)
		return (-1);
	token = new_token(word, WORD);
	free(word);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i);
}

t_tkn	*tokenize(char *line, t_sh *mini)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			i++;
		else if (line[i] == '>' && line[i + 1] == '>')
			i = tokenize_append(line, i, &mini->tkn_list);
		else if (line[i] == '>')
			i = tokenize_redir_out(line, i, &mini->tkn_list);
		else if (line[i] == '<' && line[i + 1] == '<')
			i = tokenize_heredoc(line, i, &mini->tkn_list);
		else if (line[i] == '<')
			i = tokenize_redir_in(line, i, &mini->tkn_list);
		else if (line[i] == '|')
			i = tokenize_pipe(line, i, &mini->tkn_list);
		else
			i = tokenize_complex_word(line, i, &mini->tkn_list);
		if (i < 0)
			return (NULL);
	}
	return (mini->tkn_list);
}
