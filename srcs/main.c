/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:41:52 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/15 17:04:57 by ridoming         ###   ########.fr       */
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
	copy_env(&mini, envp);
	while (1)
	{
		line = readline("Prompt >> ");
		if (!line)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			break ;
		}
		if (line[0] != '\0')
			add_history(line);
		if (parser(line, &mini))
			executor(&mini);
		free_cmd_list(mini.cmd_list);
		mini.cmd_list = NULL;
		free(line);
	}
	return (0);
}
