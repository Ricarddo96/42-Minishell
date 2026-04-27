/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:41:52 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/27 12:57:45 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		g_signal;

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_sh	mini;

	(void)argc;
	(void)argv;
	fill_struct(&mini, envp);
	while (1)
	{
		handle_signals_interactive();
		line = readline("Prompt >> ");
		if (g_signal)
			signal_status(&mini);
		if (!line)
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			break ;
		}
		if (line[0] != '\0')
			add_history(line);
		if (parser(line, &mini))
			executor(&mini);
		free(line);
	}
	return (0);
}
