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

static int	out_of_limits(unsigned long long res, int sign, const char c)
{
	if ((res > (LONG_MAX / 10) && sign == 1)
		|| (res == LONG_MAX / 10 && (c - '0') > 7))
		return (1);
	else if ((res > (unsigned long long)LONG_MAX / 10)
		|| (res == (unsigned long long)LONG_MAX / 10 && (c - '0') > 8))
		return (1);
	else
		return (0);
}

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
	unsigned long long	res;

	i = 0;
	sign = 1;
	res = 0;
	check_sign(&i, &sign, nptr);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (out_of_limits(res, sign, nptr[i]))
		{
			*error = 0;
			return (1);
		}
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	return (sign * res);
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
