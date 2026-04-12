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

int	parser(char *line, char **envp)
{
	t_tkn	*sequence;

	sequence = tokenize(line, envp);
	if (!validate(sequence))
		return (0);
	return (EXIT_SUCCESS);
}