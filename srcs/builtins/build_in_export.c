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

static void	print_export_env(char **tmp)
{
	int	j;
	int	i;

	i = 0;
	while (tmp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		j = 0;
		if (!ft_strchr(tmp[i], '='))
			ft_putendl_fd(tmp[i], STDOUT_FILENO);
		else
		{
			while (tmp[i][j])
			{
				ft_putchar_fd(tmp[i][j], STDOUT_FILENO);
				if ((tmp[i] + j) == ft_strchr(tmp[i], '='))
					ft_putchar_fd('"', STDOUT_FILENO);
				j++;
			}
			ft_putendl_fd("\"", STDOUT_FILENO);
		}
		i++;
	}
}

static void	sort_copy(char **tmp)
{
	int		i;
	char	*ptr;
	int		len;

	i = 0;
	while (tmp[i] && tmp[i + 1])
	{
		len = ft_strlen(tmp[i]);
		if (ft_strlen(tmp[i + 1]) > len)
			len = ft_strlen(tmp[i + 1]);
		if (ft_strncmp(tmp[i], tmp[i + 1], len) > 0)
		{
			ptr = tmp[i];
			tmp[i] = tmp[i + 1];
			tmp[i + 1] = ptr;
			i = 0;
		}
		else
			i++;
	}
}

static void	exec_export_no_args(t_sh *mini)
{
	char	**tmp;
	int		i;

	i = 0;
	while (mini->envp[i])
		i++;
	tmp = calloc(sizeof(char *), i + 1);
	if (!tmp)
	{
		free_cmd_list(mini->cmd_list);
		free_matrix(mini->envp);
		exit (1);
	}
	i = 0;
	while (mini->envp[i])
	{
		tmp[i] = mini->envp[i];
		i++;
	}
	sort_copy(tmp);
	print_export_env(tmp);
	free(tmp);
    mini->exit_status = 0;
}

void	which_export(t_sh *mini)
{
	if (!mini->cmd_list->args[1])
    {
		exec_export_no_args(mini);
    }
    else
	{
        exec_export_args(mini);
	}
}
