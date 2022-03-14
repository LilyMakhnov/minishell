#include "minishell.h"

int	built_in_pwd(int fd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (1);
	g_exit_status = 0;
    write(fd, pwd, ft_strlen(pwd));
    write(fd, "\n", 1);
	free(pwd);
	return (0);
}