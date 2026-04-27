/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 12:48:32 by ridoming          #+#    #+#             */
/*   Updated: 2026/04/23 16:41:02 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell
#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 700
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <curses.h>
# include <time.h>
# include <term.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <limits.h>
# include "../libft/libft.h"

extern int	g_signal;

typedef enum e_tkn_type
{
	EMPTY,
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_tkn_type;

typedef enum e_quote_state
{
	NONE,
	DOUBLE,
	SINGLE
}	t_quote_state;

typedef struct s_redir
{
	t_tkn_type		type;
	char			*file;
	struct s_redir	*next;
	int				heredoc_fd;
}	t_redir;

typedef struct s_tkn
{
	char			*token;
	t_tkn_type		type;
	struct s_tkn	*next;
	struct s_tkn	*prev;
}	t_tkn;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_minishell
{
	int		exit_status;
	t_tkn	*tkn_list;
	t_cmd	*cmd_list;
	char	**envp;
}	t_sh;

// Main
void		fill_struct(t_sh *mini, char **envp);

// Parser
int			parser(char *line, t_sh *mini);
void		copy_env(t_sh *mini, char **envp);
int			error_msg(char *msg);

// Tokenizado
t_tkn		*tokenize(char *line, t_sh *mini);
int			tokenize_heredoc(char *line, int i, t_tkn **tkn_list);
int			tokenize_redir_in(char *line, int i, t_tkn **tkn_list);
int			tokenize_redir_out(char *line, int i, t_tkn **tkn_list);
int			tokenize_append(char *line, int i, t_tkn **tkn_list);

// Crear tokens
t_tkn		*new_token(char *str, t_tkn_type type);
void		add_back(t_tkn **head, t_tkn *new_tkn);

// Validator
int			validate(t_tkn *seq, t_sh *mini);

// Expansor
void		expand(t_tkn *seq, t_sh *mini);
char		*append_var(char *result, char *token, int *i, t_sh *mini);

// Build cmd list
int			build_cmd_list(t_tkn *tokens, t_cmd **head);

// Liberar memoria
void		free_tokens(t_tkn *list);
void		free_cmd_list(t_cmd *list);
void		free_matrix(char **matrix);

// Executor
int			executor(t_sh *mini);
void		exec_one_cmd(t_sh *mini);
int			exec_pipeline(t_sh *mini);
char		*resolve_path(char *cmd, char **envp);
int			apply_redirs(t_redir *redirs);
void		free_redirs(t_redir *list);
int			count_cmds(t_cmd *cmd);
void		close_pipes(int (*pipes)[2], int count);
void		spawn_children(t_sh *mini, int (*pipes)[2], pid_t *pids, int n);

// Built-ins
void		which_built_ins(t_sh *mini);
void		exec_echo(t_sh *mini);
void		exec_pwd(t_sh *mini);
void		exec_env(t_sh *mini);
void		which_dir(t_sh *mini);
void		which_exit(t_sh *mini);
void		exec_unset(t_sh *mini);
void		which_export(t_sh *mini);
void		exec_export_args(t_sh *mini);

// Built-ins utils
int			is_built_in(char *cmd);
long long	ft_atoi_buildins(const char *nptr, int *error);
void		new_env_var(t_sh *mini, char *new_var);

// Built-ins error msg
void		print_export_error_msg(char *str);
void		print_unset_error(char *str);
void		too_many_args(t_sh *mini);

// Signals
void		handle_signals_interactive(void);
void		handle_signals_exec(void);
void		handle_signals_child(void);
void		signal_status(t_sh *mini);

// pipes
int			create_pipes(int (*pipes)[2], int n);

// Heredoc
int			pre_read_heredocs(t_cmd *cmd_list);
int			read_heredoc(char *delimiter);
void		handle_signals_heredoc(void);

#endif
