#include "minishell.h"

void    built_in_exit(t_cmd **cmd, t_env *env, int *pid)
{
    write(1, "exit", 4);
    close_fd_all(cmd);
	ft_free_cmd(cmd);
    if (pid)
    {
        free(pid);
    }
	ft_free_env(&env);
    close(0);
    close(1);
    exit(0);
}