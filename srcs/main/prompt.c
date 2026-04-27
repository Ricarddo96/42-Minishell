/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:41:52 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/21 16:39:40 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	get_prompt_home(char *env_home, char **line, char *dir)
{
	int		l;
	char	*prompt;
	char	*env;
	int		size;

	env = env_home + 5;
	l = ft_strlen(env);
	size = (ft_strlen(dir) - l) + 32;
	prompt = malloc(sizeof(char) * size);
	if (!prompt)
		exit (1);
	ft_strlcpy(prompt, "\001\033[97;44m\002Minishell:\001\033[0m\002", size);
	ft_strlcat(prompt, " ~", size);
	ft_strlcat(prompt, dir + l, size);
	ft_strlcat(prompt, "$ ", size);
	(*line) = readline(prompt);
	free(prompt);
	return (1);
}

static int	search_home(char **env, char *dir, int *i)
{
	int	j;

	while (env[(*i)])
	{
		if (ft_strncmp(env[(*i)], "HOME=", 5) == 0)
			break ;
		(*i)++;
	}
	if (env[(*i)] != NULL)
	{
		j = ft_strlen(env[(*i)] + 5);
		if (ft_strncmp(dir, env[(*i)] + 5, j) == 0)
		{
			if (dir[j] == '\0' || dir[j] == '/')
				return (1);
		}
	}
	return (0);
}

void	get_prompt(t_sh *mini, char **line)
{
	char	dir[PATH_MAX];
	char	*prompt;
	int		i;

	i = 0;
	if (getcwd(dir, sizeof(dir)) == NULL)
	{
		(*line) = readline("Minishell >> ");
		return ;
	}
	search_home(mini->envp, dir, &i);
	if (get_prompt_home(mini->envp[i], line, dir))
		return ;
	prompt = malloc(sizeof(char) * (ft_strlen(dir) + 31));
	if (!prompt)
	{
		free_matrix(mini->envp);
		exit(1);
	}
	ft_strlcpy(prompt, "\001\033[97;44m\002Minishell:\001\033[0m\002", 28);
	ft_strlcat(prompt, dir, (ft_strlen(dir) + 28));
	ft_strlcat(prompt, "$ ", (ft_strlen(prompt) + 3));
	(*line) = readline(prompt);
	free(prompt);
}
