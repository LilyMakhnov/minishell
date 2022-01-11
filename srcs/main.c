/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esivre <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:10:32 by esivre            #+#    #+#             */
/*   Updated: 2022/01/11 18:11:30 by esivre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_cmd(char **cmd)
{
	pid_t	pid;
	int		status = 0;

	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid > 0) {
		waitpid(pid, &status, 0);
		kill(pid, SIGTERM);
	} else {
		if (execve(cmd[0], cmd, NULL) == -1)
			perror("shell");
		exit(EXIT_SUCCESS);
	}
}

void	free_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

int	main()
{
	char	*line;
	char	**cmd;

	line = NULL;
	cmd = NULL;
	write(1, "$> ", 3);
	while (get_next_line(0, &line) > 0)
       	{
		cmd = ft_split(line, ' ');
		free(line);
		exec_cmd(cmd);
		free_array(cmd);
		line = NULL;
		write(1, "$> ", 3);
	}
}

