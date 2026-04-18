/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 15:43:57 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/16 15:45:29 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	check_sign(int *i, int *sign, const char *str)
{
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-' )
			*sign = -1;
		(*i)++;
	}
}

long long	ft_atoi_buildins(const char *nptr, int *error)
{
	int					i;
	int					sign;
	unsigned long long	resultado;

	i = 0;
	sign = 1;
	resultado = 0;
	check_sign(&i, &sign, nptr);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		resultado = resultado * 10 + (nptr[i] - '0');
		i++;
		if (resultado > LONG_MAX && sign == 1)
		{
			*error = 0;
			return (1);
		}
		if (resultado > ((unsigned long long)LONG_MAX + 1) && sign == -1)
		{
			*error = 0;
			return (1);
		}
	}
	return (sign * resultado);
}

int	is_num(char *arg)
{
	int	i;

	i = 0;
	if (!arg[i])
		return (0);
	if (arg[i] == '-' || arg[i] == '+')
	{
		i++;
		if (!arg[i])
			return (0);
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	new_env_var(t_sh *mini, char *new_var)
{
	char	**new_env;
	int		i;

	i = 0;
	while (mini->envp[i])
		i++;
	new_env = calloc(i + 2, sizeof(char *));
	if (!new_env)
		exit (1);
	i = 0;
	while (mini->envp[i])
	{
		new_env[i] = mini->envp[i];
		i++;
	}
	new_env[i] = new_var;
	free(mini->envp);
	mini->envp = new_env;
}

int	is_built_in(char *cmd)
{
	if (!ft_strncmp(cmd, "echo", 5)
		|| !ft_strncmp(cmd, "cd", 3)
		|| !ft_strncmp(cmd, "pwd", 4)
		|| !ft_strncmp(cmd, "export", 7)
		|| !ft_strncmp(cmd, "unset", 6)
		|| !ft_strncmp(cmd, "env", 4)
		|| !ft_strncmp(cmd, "exit", 5))
		return (1);
	return (0);
}
