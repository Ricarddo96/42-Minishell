/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:05:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/03/18 12:53:28 by ridoming         ###   ########.fr       */
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
//Probar esta función
int	tokenize_quotes(char *line, int i, t_tkn **tkn_list)
{
	int		start;
	char	which_quote;
	char	*str;
	t_tkn	token;

	which_quote = line[i];
	start = i;
	i++;
	while (line[i] && line[i] != which_quote)
		i++;
	if (line[i] == '\0')
		return (-1);
	while (line[i + 1] && line[i + 1] != '"' line[i + 1] != 39
		&& line[i + 1] != '>' && line[i + 1] != '<' line[i + 1] != '|'
		&& line[i] != ' ')
		i++;
	str = ft_substr(line, start, i - start);
	if (!str)
		return (-1);
	token = new_token(str, WORD);
	free(str);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 1);
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

int	tokenize_word(char *line, int i, t_tkn **tkn_list)
{
	int		start;
	char	*word;
	t_tkn	*token;

	start = i;
	while (line[i] && line[i] != ' '
		&& line[i] != '|'
		&& line[i] != '<'
		&& line[i] != '>')
		i++;
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

t_tkn	*tokenize(char *line)
{
	t_tkn	*tkn_list;
	int		i;

	tkn_list = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (line[i] == '"' || line[i] == '\'')
			i = tokenize_quotes(line, i, &tkn_list);
		else if (line[i] == '>' && line[i + 1] == '>')
			i = tokenize_append(line, i, &tkn_list);
		else if (line[i] == '>')
			i = tokenize_redir_out(line, i, &tkn_list);
		else if (line[i] == '<' && line[i + 1] == '<')
			i = tokenize_heredoc(line, i, &tkn_list);
		else if (line[i] == '<')
			i = tokenize_redir_in(line, i, &tkn_list);
		else if (line[i] == '|')
			i = tokenize_pipe(line, i, &tkn_list);
		else
			i = tokenize_word(line, i, &tkn_list);
	}
	return (tkn_list);
}
