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
		exit(1);
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

char	**get_path(char **env)
{
	int i;
	char *path;
	char **ret;

	i = -1;
	while (env[++i])
		if(!ft_strncmp(env[i], "PATH=", 5))
		{
			path = ft_strdup(env[i] + 5);
			ret = ft_split(path, ':');
			free(path);
			return (ret);
		}
	return (NULL);
}

char	*ft_strtrijoin(char *str1, char *str2, char *str3)
{
	char *bin;

	bin = malloc(ft_strlen(str1) + ft_strlen(str2) + ft_strlen(str3));
	bin[0] = 0;
	ft_strlcat(bin, str1, ft_strlen(str1) + 1);
	ft_strlcat(bin, str2, ft_strlen(bin) + ft_strlen(str2) + 1);
	ft_strlcat(bin, str3, ft_strlen(bin) + ft_strlen(str3) + 1);
	return (bin);
}
int	main(int argc, char **argv, char **env)
{
	char	*line;
	char	**cmd;
	char	**path;
	char	*bin;
	int 	i;

	(void)argc;
	(void)argv;
	line = NULL;
	cmd = NULL;
	path = get_path(env);
	write(1, "$> ", 3);
	while (get_next_line(0, &line) > 0)
    {
		cmd = ft_split(line, ' ');
		free(line);
		line = NULL;
		if (!cmd)
		{
			free_array(path);
			exit(1);
		}
		if (!access(cmd[0], F_OK))
			exec_cmd(cmd);
		else if (cmd[0])
		{
			i = -1;
			while (path[++i])
			{
				bin = ft_strtrijoin(path[i], "/", cmd[0]);	
				if (!bin)
				{
					free_array(path);
					free_array(cmd);
					exit(1);
				}		
				if (!access(bin, F_OK))
				{
					free(cmd[0]);
					cmd[0] = bin;
					exec_cmd(cmd);
					break;
				}
				free(bin);
				bin = NULL;
			}
			if (!bin)
				write(1, "command not found\n", 18);
		}
		free_array(cmd);
		write(1, "$> ", 3);
	}
	free_array(path);
	return (0);
}

