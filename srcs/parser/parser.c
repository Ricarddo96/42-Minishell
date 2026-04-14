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
#include <stdlib.h>



t_cmd *new_cmd(void)
{
	t_cmd *new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->next = NULL;
	new->redirs = NULL;
	return (new);
}
int add_arg(t_cmd *cmd, char *token)
{
	char **new_args;
	int count;
	int i;
	
	count = 0;
	i = 0;
	if (cmd->args)
		while (cmd->args[count])
			count++; 
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (0);
	while(i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_strdup(token);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (1);	
}

int add_redir(t_cmd *cmd, t_tkn_type type, char *file)
{
	t_redir	*new;
    t_redir	*last;
	
    new = malloc(sizeof(t_redir));
    if (!new)
        return (0);
	new->next = NULL;
	new->type = type;
	new->file = ft_strdup(file);
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		last = cmd->redirs;
		while (last->next) 
			last = last->next;
		last->next = new;
	}
	return (1);
}

t_cmd *build_cmd_list(t_tkn *tkn_list)
{
	t_cmd *head;
	t_cmd *current;

	head = new_cmd();
	if (!head)
		return (NULL);
	current = head;
	while(tkn_list)
	{
		if (tkn_list->type == WORD)
			add_arg(current, tkn_list->token);
		else if (tkn_list->type == PIPE)
		{
			current->next = new_cmd();
			if (!current->next)
				return (NULL);
			current = current->next;			
		}
		else 
		{
			add_redir(current, tkn_list->type, tkn_list->next->token);
			tkn_list = tkn_list->next;
		}
		tkn_list = tkn_list->next;
	}
	return (head);
}

void	free_tokens(t_tkn *list)
{
	t_tkn	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->token);
		free(list);
		list = tmp;
	}
}

void	free_redirs(t_redir *list)
{
	t_redir	*tmp;

	while (list)
	{
		tmp = list->next;
		free(list->file);
		free(list);
		list = tmp;
	}
}

void	free_cmd_list(t_cmd *list)
{
	t_cmd	*tmp;
	int		i;

	while (list)
	{
		tmp = list->next;
		if (list->args)
		{
			i = 0;
			while (list->args[i])
				free(list->args[i++]);
			free(list->args);
		}
		free_redirs(list->redirs);
		free(list);
		list = tmp;
	}
}

int	parser(char *line, t_sh *mini)
{
	t_tkn	*tmp;

	if (!validate_quotes(line))
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
	mini->cmd_list = build_cmd_list(mini->tkn_list);
	free_tokens(mini->tkn_list);
	mini->tkn_list = NULL;
	return (1);
}
