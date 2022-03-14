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
#include <fcntl.h>

typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_cmd t_cmd;

typedef enum token
{
    NONE, //defaut set
    ARG, //word
    FILE_IN, //word == '<'
    HERE_DOC, // word == '<<'
    FILE_OUT, //word == '>'
    FILE_OUT_SUR, //word == '>>'
    OPEN_FILE, // word following '<'
    LIMITOR, // word following '<<'
    EXIT_FILE, // word followinf '>' FILE_OUT
    EXIT_FILE_RET // word following '>>'  FILE_OUT_SUR
}	t_e_token;

struct s_cmd
{
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
	t_e_token	type;
	t_token *next;
};

struct s_env
{
	char *value;
	t_env *next;
};

extern int g_exit_status;


void    built_in_exit(t_cmd **cmd, t_env *env, int *pid);
int	built_in_pwd(int fd);

////////////////////////BUILT_IN_CD////////////////////////////
void	built_in_cd(char *path);

////////////////////////BUILT_IN_ECHO////////////////////////////

//int	print_var(char *str, t_env *env);
//int parse_var(char *cmd, int *j, t_env *env);
//void	built_in_echo(char **cmd, t_env *env);
int	print_var(char *str, t_env *env, int fd);
void	built_in_echo(char **cmd, int fd, t_env *env);
int	parse_var(char *cmd, int *j, t_env *env, int fd);

////////////////////////BUILT_IN_ENV/////////////////////////////
char	**get_path(char **env);
void	lst_remove_var(t_env **env, char *var);
void	lst_set_var(t_env **env, char *var);
int	lst_get_var(t_env *env, char *var, char **value);
char	*get_var(char **env, char *var);

///////////////////////CHECK_PIPE////////////////////////////////

int	check_pipe_err(char *str, int i);
int	check_syntax_pipe(char *rdl);

///////////////////////CHECK_TOKEN//////////////////////////////
int	check_token_cmd(t_cmd *cmd);
int	check_token(t_token *token);

//////////////////////////EXPAND////////////////////////////////
int	expand_var(char **str, int i, t_env *env);
int	expand_str(char **str, t_env *env);
int	expand_cmd_token(t_cmd *cmd, t_env *env);
int	lexer_build(char *cmd, t_token **src);
int	get_value_str(char *str, int i, char **value, t_env *env);
int	ft_var_name_len(char *str);

//

///////////////////////////FREE/////////////////////////////////
void	free_three_str(char *str1, char *str2, char *str3);
void	ft_free_env(t_env **liste);
void	ft_free_token(t_token **token);
void	ft_free_cmd(t_cmd **cmd);
void	free_array(char **array);

//////////////////////////LEXER/////////////////////////////////
int	lexer_general_std(char a, int *state, t_token **token);
int	lexer_general_op(char *str, int *i, t_token **token);
int	lexer_general(char *str, int *i, t_token **token, int *state);
int	lexer_quote(char *line, int i, t_token **token, int *state);
t_e_token	upgrade_type(t_e_token prev, t_e_token curr);
int	is_type_operator(t_e_token type);

////////////////////////LLIST_CREATE////////////////////////////
t_env	*ft_lst_create(char *value);
t_env	*ft_lst_last(t_env *lst);
void	ft_lst_pushback(t_env **liste, t_env *element);

///////////////////////LLIST_ENV///////////////////////////////

int	ft_envlen(t_env *env);
char	**ft_env_from_lst(t_env *llenv);
t_env	*ft_lst_env(char **env);
void	lst_print(t_env *env, int fd);

////////////////////////MINISHELL//////////////////////////////

void	close_fd_all(t_cmd **cmdl);

//////////////////////////PRINT///////////////////////////////

void	print_cmd(t_cmd *cmd);
void	print_token(t_token *token);
void	print_cmd_token(t_cmd *cmd);

///////////////////////////QUOTES/////////////////////////////
int	is_open_quote(char *rdl);
void	ft_update_quotes(char *str, int i, int *quotes);
int	cmd_token(t_cmd **cmd);

/////////////////////////SPLIT_CMD////////////////////////////
int	get_index_pipe(char *rdl);
int	add_next_cmd(char *rdl, t_cmd **cmd, int *ip);
int	split_cmd(char *rdl, t_cmd **cmd);
int	nbr_cmd(t_cmd *cmd);
t_cmd	*get_i_cmd(t_cmd *cmd, int j);

//////////////////////////TOKEN//////////////////////////////

t_token	*ft_lst_create_token(char *data);
int	is_char_in_set(char c, char *set);
int	new_token_next(t_token **token);
t_e_token	get_type(char *str, int i);
void	unexpected_token(char *str);

//////////////////////////UTILS///////////////////////////////
char	*ft_strtrijoin(char *str1, char *str2, char *str3);
char	*ft_strndup(const char *src, size_t n);



///other
char **ft_env_from_lst(t_env *llenv);
void	free_token(t_token **token);
void		free_cmd(t_cmd **cmd);


void	free_env(t_env **liste);

#endif
