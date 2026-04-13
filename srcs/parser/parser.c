/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:58:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/12 15:30:47 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parser(char *line, t_sh mini)
{
	mini.tkn_list = tokenize(line, mini);
	while (mini.tkn_list != NULL)
	{
		printf("%s\n", mini.tkn_list->token);
		mini.tkn_list = mini.tkn_list->next;
	}
	if (!validate(mini.tkn_list))
		return (0);
	return (EXIT_SUCCESS);
}