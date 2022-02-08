#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_cmd t_cmd;

struct s_cmd
{
	char *cmd;
	int fd_in;
	int fd_out;
	t_token *token;
	t_cmd *next;
};

struct s_token
{
	char	*data;
	int		type;
	t_token *next;
};

struct s_env
{
	char *value;
	t_env *next;
};

//split_cmd.c
int	get_index_pipe(char *rdl);
int	add_next_cmd(char *rdl, t_cmd **cmd);
int	split_cmd(char *rdl, t_cmd **cmd);
void	print_cmd(t_cmd *cmd);  //debug

t_env	*ft_lst_env(char **env);
t_env	*ft_lst_create(char *value);
t_token	*ft_lst_create_token(char *data, int type);
void	lexer_general(char *str, int *i, t_token **token, int *state);
void	lexer_build(char* line, t_token **src);
int		is_char_in_set(char c, char *set);
void	ft_free_linkedlist(t_env **liste);
void	ft_lst_pushback(t_env **liste, t_env *element);
t_env	*ft_lst_last(t_env *lst);

#endif
