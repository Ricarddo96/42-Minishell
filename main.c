/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:41:52 by ridoming          #+#    #+#             */
/*   Updated: 2026/03/11 13:35:50 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(void)
{
    char *linea;
    linea = readline("Prompt > ");
    printf("%s\n", linea);
    free(linea);
    return (0);
}

/* readline es una función bloqueante: se queda esperando input del usuario hasta que pulsa Enter. Solo entonces devuelve el string y el programa continúa con el printf */