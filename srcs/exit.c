#include "minishell.h"

void    built_in_exit(t_cmd **cmd, t_env *env, int *pid)
{
    write(2, "exit\n", 5);
    close_fd_all(cmd);
	ft_free_cmd(cmd);
    if (pid)
    {
        free(pid);
    }
	ft_free_env(&env);
    exit(0);
}