#include "minishell.h"

void	built_in_cd(char *path)
{
	if (chdir(path) == -1)
    {
		perror("chdir()");
	}
}