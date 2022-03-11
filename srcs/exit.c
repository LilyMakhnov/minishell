#include "minishell.h"

void    built_in_exit(void)
{
    write(1, "exit", 4);
    exit(0);
}