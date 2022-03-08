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
//	char *cmd;
	char *str;
	int fd_in;
	int fd_out;
	char **arg;
	char **env;
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

char	*ft_strtrijoin(char *str1, char *str2, char *str3);
int		lst_get_var(t_env *env, char *var, char **value);
char **ft_env_from_lst(t_env *llenv);
void	free_token(t_token **token);
void		free_cmd(t_cmd **cmd);
int	print_var(char *str, t_env *env);
int parse_var(char *cmd, int *j, t_env *env);
void	built_in_echo(char **cmd, t_env *env);

char	*get_var(char **env, char *var);
//char	*lst_get_var(t_env *env, char *var);
void	lst_set_var(t_env **env, char *var);
void	lst_remove_var(t_env **env, char *var);
char	**get_path(char **env);
void	lst_print(t_env *env, int fd);


//split_cmd.c
int	get_index_pipe(char *rdl);
int	add_next_cmd(char *rdl, t_cmd **cmd);
int	split_cmd(char *rdl, t_cmd **cmd);
void	print_cmd(t_cmd *cmd);  //debug

t_env	*ft_lst_env(char **env);
t_env	*ft_lst_create(char *value);
t_token	*ft_lst_create_token(char *data, int type);
int		lexer_general(char *str, int *i, t_token **token, int *state);
int		lexer_build(char* line, t_token **src);
int		is_char_in_set(char c, char *set);
//void	ft_free_linkedlist(t_env **liste);
void	ft_lst_pushback(t_env **liste, t_env *element);
t_env	*ft_lst_last(t_env *lst);
void	free_env(t_env **liste);
void	ft_free_linkedlist(t_env **liste);

#endif
