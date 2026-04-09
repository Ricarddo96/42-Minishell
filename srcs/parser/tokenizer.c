/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 14:05:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/08 20:32:55 by ridoming         ###   ########.fr       */
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

void append_char(char **clean, char c)
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

// char *process_token(char *raw_token,  char **envp)
// {
// 	t_quote_state	state;
// 	char 			*clean;
// 	int				i;

// 	i = 0;
// 	clean = NULL;
// 	state = NONE;
// 	while (raw_token[i]) 
// 	{
// 		if (state == NONE)
// 		{
// 			if (raw_token[i] == '"')
// 	 			state = DOUBLE;
// 			else if (raw_token[i] == '\'') 
// 				state = SINGLE;
// 			else if (raw_token[i] == '$')
// 				i = i + expand_var(raw_token + i, &clean, envp);
// 			else                     
// 				append_char(&clean, raw_token[i]);
// 		}
// 		else if (state == DOUBLE)
// 		{
// 			if (raw_token[i] == '"')
// 				state = NONE;
// 			else if (raw_token[i] == '$') 
// 				i = i + expand_var(raw_token + i, &clean, envp);
// 			else
// 				append_char(&clean, raw_token[i]);
// 		}
// 		else if (state == SINGLE)
// 		{
// 			if (raw_token[i] == '\'')
// 				state = NONE;
// 			else
// 			 	append_char(&clean, raw_token[i]);
// 		}
// 		i++;
// 	}
// 	return (clean);
// }


/*	Esto lo que hace es iterar por los strings de envp y solo se pone a iterar en envp cuando la primera letra de un string
	coincida con la primera de raw, cuando coincide empiezo a iterar mientras raw coincida con envp, sin modificar i,
	Si al acabar el bucle envp está en '=' qiuiere decir que la variable es esa, hago un break, y empiezo a agregar
	los chars a raw, devuelvo l que es con lo que he iterado en raw - i, porque la llamada a esta funcion 
	hace i = i +expand_var, entocnes tengoo que devolver solo lo que iterado en raw, que es l - i

*/
int expand_var(char *raw, int i, char **clean, char **envp)
{
	int j;
	int k;
	int	l;
	
	j = 0;
	while (envp[j])
	{
		k = 0;
		if (envp[j][k] == raw[i])
		{
			l = i + 1;
			while (envp[j][k] == raw[l])
			{
				k++;
				l++;
			}
			if (envp[j][k] == '=')
				break ;
		}
		j++;
	}
	while (envp[j][k + 1] != '\0' )
		append_char(&clean, envp[j][++k]);
	return (l - i);
}

/*	Mira este process_token a ver que te parece, que así si tiene las menos de 25 lineas y funciona igual (en principio)
	En los primeros 4 ifs se mira si el caracter actual es algun tipo de comilla y si ya habia una antes o no,
	si no habia se pone como que se abre e i++, si sí estaban abiertas las comillas se cierran.
	Luego se comprueba si es una varible y si quotes no es single, que es la unica vez en la que la varible no se expande
	y en cualquier otro caso se pone el caracter.
*/
char *process_token(char *raw_token,  char **envp)
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
		else if (raw_token[i] == '/' && quotes == NONE)
			quotes = SINGLE;
		else if (raw_token[i] == '"' && quotes == DOUBLE)
			quotes = NONE;
		else if (raw_token[i] == '/' && quotes == SINGLE)
			quotes = NONE;
		else if (raw_token[i] == '$' && quotes != SINGLE)
			i += expand_var(raw_token, i, &clean, envp);
		else
			append_char(&clean, raw_token[i]);
		i++;
	}
	return (clean);
}

int	tokenize_complex_word(char *line, int i, t_tkn **tkn_list, char **envp)
{
	int		start;
	char	*word;
	t_tkn	*token;
	int		vars_in_str;

	start = i;
	vars_in_str = 0;
	while (line[i] && line[i] != '<' && line[i] != '>' && line[i] != '|'
		&& line[i] != ' ')
	{
		if (line[i] == '$')
			vars_in_str = 1;
		i++;
	}
	word = ft_substr(line, start, i - start);
	if (!word)
		return (-1);
	if (vars_in_str)
		word = process_token(word, envp);
	token = new_token(word, WORD);
	free(word);
	if (!token)
		return (-1);
	add_back(tkn_list, token);
	return (i + 1);
}

t_tkn	*tokenize(char *line, char **envp)
{
	t_tkn	*tkn_list;
	int		i;

	tkn_list = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			i++;
		else if (line[i] != ' ' && line[i] != '|'
			&& line[i] != '<' && line[i] != '>')
			i = tokenize_complex_word(line, i, &tkn_list, envp);
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
	}
	return (tkn_list);
}
