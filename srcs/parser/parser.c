/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:58:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/14 13:44:06 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		return (0);
	expand(mini->tkn_list, mini);
	tmp = mini->tkn_list;
	while (tmp != NULL)
	{
		printf("%s\n", tmp->token);
		tmp = tmp->next;
	}
	return (1);
}