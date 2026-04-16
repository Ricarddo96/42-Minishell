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

//esta fucnion checkea si es un built in, la necesitaba para el ejecutor
int is_built_in(char *cmd)
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