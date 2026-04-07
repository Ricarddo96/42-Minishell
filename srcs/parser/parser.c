/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/19 16:58:24 by ridoming          #+#    #+#             */
/*   Updated: 2026/03/19 16:58:53 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parser(char *line, char **envp)
{
	t_tkn	*sequence;

	sequence = tokenize(line);
	if (!validate(sequence))
		return (0);
	sequence = env_expand(sequence, envp);
	return (EXIT_SUCCESS);
}