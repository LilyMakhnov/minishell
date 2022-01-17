#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
//#include <readline/readline.h>
//#include <readline/history.h>

typedef struct s_env	t_env;

struct s_env
{
	char *value;
	t_env *next;
};

t_env	*ft_lst_env(char **env);
t_env	*ft_lst_create(char *value);
void	ft_free_linkedlist(t_env **liste);
void	ft_lst_pushback(t_env **liste, t_env *element);
t_env	*ft_lst_last(t_env *lst);

#endif
