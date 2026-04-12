/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:48:32 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/08 19:22:14 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <termios.h>
#include <curses.h>
#include <time.h>

#include <term.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

// lo ideal es que el parser rellene una *lista enlazada* donde cada nodo sea un comando separado por un pipe.

typedef enum e_tkn_type
{
    EMPTY,            // 0 - Útil para inicializar
    WORD,             // 1 - Comandos, argumentos, nombres de archivo
    PIPE,             // 2 - '|'
    REDIR_IN,        // 3 - '<'
    REDIR_OUT,       // 4 - '>'
    REDIR_APPEND,    // 5 - '>>'
    REDIR_HEREDOC    // 6 - '<<'
}   t_tkn_type;

typedef enum e_quote_state
{
    NONE,
    DOUBLE,
    SINGLE
}   t_quote_state;

typedef struct s_tkn
{
    char            *token;
    t_tkn_type      type;
    int             single_quoted;
    struct s_tkn    *next;
    struct s_tkn    *prev;
}   t_tkn;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	struct s_cmd	*next;
}	t_cmd;

t_tkn	*new_token(char *str, t_tkn_type type);
void	add_back(t_tkn **head, t_tkn *new_tkn);
int		tokenize_append(char *line, int i, t_tkn **tkn_list);
int		tokenize_redir_out(char *line, int i, t_tkn **tkn_list);
int		tokenize_heredoc(char *line, int i, t_tkn **tkn_list);
int		tokenize_redir_in(char *line, int i, t_tkn **tkn_list);
int		tokenize_pipe(char *line, int i, t_tkn **tkn_list);
t_tkn	*tokenize(char *line, char **envp);
int		validate(t_tkn *seq);
int		parser(char *line, char **envp);

#endif
