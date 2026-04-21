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

#include "../../includes/minishell.h"

void	fill_struct(t_sh *mini, char **envp)
{
	mini->exit_status = 0;
	mini->tkn_list = NULL;
	copy_env(mini, envp);
}
