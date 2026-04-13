/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:05:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/12 15:59:12 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tkn	*new_token(char *str, t_tkn_type type)
{
	t_tkn	*node;

	node = malloc(sizeof(t_tkn));
	if (!node)
		return (NULL);
	node->token = ft_strdup(str);
	if (!node->token)
	{
		free(node);
		return (NULL);
	}
	node->type = type;
	node->single_quoted = 0; // marcamos por defecto que no tiene comillas simples, en tokenize_quotes despues de llamar a esta funcion habra que hacer node->single_quoted = 1; (esto es porque no se deben expandir variables con comillas simples)
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	add_back(t_tkn **head, t_tkn *new_tkn)
{
	t_tkn	*copy;

	if (head == NULL || new_tkn == NULL)
		return ;
	if (*head == NULL)
	{
		*head = new_tkn;
		return ;
	}
	copy = *head;
	while (copy->next != NULL)
		copy = copy->next;
	copy->next = new_tkn;
	new_tkn->prev = copy;
}

int	tokenize_append(char *line, int i, t_tkn **tkn_list)
{
	t_tkn	*token;

	(void)line;
	token = new_token(">>", REDIR_APPEND);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 2);
}

int	tokenize_redir_out(char *line, int i, t_tkn **tkn_list)
{
	t_tkn	*token;

	(void)line;
	token = new_token(">", REDIR_OUT);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 1);
}

int	tokenize_heredoc(char *line, int i, t_tkn **tkn_list)
{
	t_tkn	*token;

	(void)line;
	token = new_token("<<", REDIR_HEREDOC);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 2);
}

int	tokenize_redir_in(char *line, int i, t_tkn **tkn_list)
{
	t_tkn	*token;

	(void)line;
	token = new_token("<", REDIR_IN);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 1);
}

int	tokenize_pipe(char *line, int i, t_tkn **tkn_list)
{
	t_tkn	*token;

	(void)line;
	token = new_token("|", PIPE);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 1);
}

static void append_char(char **clean, char c)
{
	int i;
	int j;
	char *tmp;
	
	i = 0;
	j = 0;
	tmp = NULL;
	if (*clean == NULL)
	{
		tmp = malloc(1 * sizeof(char) + 1);
		tmp[i] = c;
		tmp[i + 1] = '\0';
	}
	else 
	{
		tmp = malloc(ft_strlen(*clean) + 2);
		while ((*clean)[i])
			tmp[j++] = (*clean)[i++];
		tmp[j] = c;
		tmp[j + 1] = '\0';	
	}
	free(*clean);
	*clean = tmp;
}

static void append_expanded(char *envp_str, int equal_index, char **clean)
{
	int i;

	i = equal_index + 1;
	while (envp_str[i]) 
	{
		append_char(clean, envp_str[i]);
		i++;
	}
}
static int var_not_found(int i, int equal_index, char *raw)
{
	while (raw[i + 1 + equal_index]
		&& (ft_isalnum(raw[i + 1 + equal_index])
			|| raw[i + 1 + equal_index] == '_'))
		equal_index++;
	return (equal_index);
}

static int check_in_envp(char *raw, int i, char **clean, t_sh *mini)
{
	int	j;
	int	equal_index;

	j = 0;
	while (mini->envp[j])
	{
		equal_index = 0;
		if (mini->envp[j][0] == raw[i + 1])
		{
			while (mini->envp[j][equal_index] != '=')
				equal_index++;
			if (ft_strncmp(raw + (i + 1), mini->envp[j], equal_index) == 0
				&& !ft_isalnum(raw[i + 1 + equal_index])
				&& raw[i + 1 + equal_index] != '_')
			{
				append_expanded(mini->envp[j], equal_index, clean);
				return (equal_index);
			}
		}
		j++;
	}
	equal_index = var_not_found(i, equal_index, raw); 
	return (equal_index);
}

static char *process_token(char *raw_token, t_sh *mini)
{
	t_quote_state	quotes;
	char			*clean;
	int				i;

	i = 0;
	clean = NULL;
	quotes = NONE;
	while (raw_token[i]) 
	{
		if (raw_token[i] == '"' && quotes == NONE)
			quotes = DOUBLE;
		else if (raw_token[i] == '\'' && quotes == NONE)
			quotes = SINGLE;
		else if (raw_token[i] == '"' && quotes == DOUBLE)
			quotes = NONE;
		else if (raw_token[i] == '\'' && quotes == SINGLE)
			quotes = NONE;
		else if (raw_token[i] == '$' && quotes != SINGLE)
			i += check_in_envp(raw_token, i, &clean, mini);
		else
			append_char(&clean, raw_token[i]);
		i++;
	}
	return (clean);
}

static void	vars_in_str(char **word, t_sh *mini)
{
	char	*tmp;

	tmp = process_token(*word, mini);
	free(*word);
	*word = tmp;
}

static int	tokenize_complex_word(char *line, int i, t_sh *mini)
{
	int		start;
	char	*word;
	t_tkn	*token;
	int		has_var;

	start = i;
	has_var = 0;
	while (line[i] && line[i] != '<' && line[i] != '>' && line[i] != '|'
		&& line[i] != ' ')
	{
		if (line[i] == '$')
			has_var = 1;
		i++;
	}
	word = ft_substr(line, start, i - start);
	if (!word)
		return (-1);
	if (has_var)
		vars_in_str(&word, mini);
	token = new_token(word, WORD);
	free(word);
	if (!token)
		return (-1);
	add_back(&mini->tkn_list, token);
	return (i);
}

t_tkn	*tokenize(char *line, t_sh mini)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (line[i] == '>' && line[i + 1] == '>')
			i = tokenize_append(line, i, &mini.tkn_list);
		else if (line[i] == '>')
			i = tokenize_redir_out(line, i, &mini.tkn_list);
		else if (line[i] == '<' && line[i + 1] == '<')
			i = tokenize_heredoc(line, i, &mini.tkn_list);
		else if (line[i] == '<')
			i = tokenize_redir_in(line, i, &mini.tkn_list);
		else if (line[i] == '|')
			i = tokenize_pipe(line, i, &mini.tkn_list);
		else
			i = tokenize_complex_word(line, i, &mini);
		if (i < 0)
			return (NULL);
	}
	return (mini.tkn_list);
}
