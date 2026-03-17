/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:41:52 by ridoming          #+#    #+#             */
/*   Updated: 2026/03/17 17:45:34 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parser(char *line)
{
	t_tkn	*sequence;

	sequence = tokenize(line);
	(void)sequence;
	return (EXIT_SUCCESS);
}

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("Prompt >> ");
		add_history(line);
		parser(line);
		free(line);
	}
	return (0);
}
