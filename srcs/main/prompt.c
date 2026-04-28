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

static int	get_prompt_home(char *env_home, char **line, char *dir, t_sh *mini)
{
	int		l;
	char	*prompt;
	char	*env;
	int		size;

	env = env_home + 5;
	l = ft_strlen(env);
	size = (ft_strlen(dir) - l) + 100;
	prompt = malloc(sizeof(char) * size);
	if (!prompt)
		exit (1);
	ft_strlcpy(prompt, "\001\033[1;32m\002➜  ", size);
	ft_strlcat(prompt, "\001\033[1;36m\002minishell:\001\033[94m\002", size);
	ft_strlcat(prompt, " ~", size);
	ft_strlcat(prompt, dir + l, size);
	if (mini->exit_status == 0)
		ft_strlcat(prompt, "\001\033[1;32m\002 ❯ \001\033[0m\002", size);
	else
		ft_strlcat(prompt, "\001\033[1;31m\002 ❯ \001\033[0m\002", size);		
	(*line) = readline(prompt);
	free(prompt);
	return (1);
}

static void	search_home(char **env, char *dir, int *i)
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
				return ;
		}
	}
	return ;
}

static void	get_prompt_no_home(t_sh *mini, char **line, char *dir)
{
	char	*prompt;
	int		size;

	size = (ft_strlen(dir)) + 100;
	prompt = malloc(sizeof(char) * size);
	ft_strlcpy(prompt, "\001\033[1;32m\002➜  ", size);
	ft_strlcat(prompt, "\001\033[1;36m\002minishell:\001\033[94m\002", size);
	ft_strlcat(prompt, dir, size);
	if (mini->exit_status == 0)
		ft_strlcat(prompt, "\001\033[1;32m\002 ❯ \001\033[0m\002", size);
	else
		ft_strlcat(prompt, "\001\033[1;31m\002 ❯ \001\033[0m\002", size);
	(*line) = readline(prompt);
	free(prompt);
}

void	get_prompt(t_sh *mini, char **line)
{
	char	dir[PATH_MAX];
	int		i;

	i = 0;
	if (getcwd(dir, sizeof(dir)) == NULL)
	{
		(*line) = readline("Minishell >> ");
		return ;
	}
	search_home(mini->envp, dir, &i);
	if (mini->envp[i])
	{
		get_prompt_home(mini->envp[i], line, dir, mini);
		return ;
	}
	else
	{
		get_prompt_no_home(mini, line, dir);
		return ;
	}
}
