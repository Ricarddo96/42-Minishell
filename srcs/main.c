/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:41:52 by ridoming          #+#    #+#             */
/*   Updated: 2026/03/19 16:58:21 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_sh	mini;	

	(void)argc;
	(void)argv;
	mini.exit_status = 0;
	mini.tkn_list = NULL;
	mini.envp = envp;
	while (1)
	{
		line = readline("Prompt >> ");
		add_history(line);
		parser(line, mini);
		free(line);
	}
	return (0);
}
