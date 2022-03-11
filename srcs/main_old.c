/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_old.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 18:10:32 by esivre            #+#    #+#             */
/*   Updated: 2022/03/09 14:09:19 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

void	exec_cmd(char **cmd, char **env)
{
	pid_t	pid;
	int		status = 0;

	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid > 0) 
	{
		waitpid(pid, &status, 0);
		kill(pid, SIGTERM);
	} 
	else 
	{
		if (execve(cmd[0], cmd, env) == -1)
			perror("shell");
		exit(1);
	}
}

/*
char	*ft_strtrijoin(char *str1, char *str2, char *str3)
{
	char *bin;

	bin = malloc(ft_strlen(str1) + ft_strlen(str2) + ft_strlen(str3) + 1);
	if (!bin)
		return (NULL);
	ft_strlcpy(bin, str1, ft_strlen(str1) + 1);
	ft_strlcat(bin, str2, ft_strlen(bin) + ft_strlen(str2) + 1);
	ft_strlcat(bin, str3, ft_strlen(bin) + ft_strlen(str3) + 1);
	return (bin);
}*/



int	main(int argc, char **argv, char **env)
{
	//t_token	*token;
	t_env	*link_env;
	char	**new_env;
	char	*line;
	char	**cmd;
	char	**path;
	char	*bin;
	int 	i;

	(void)argc;
	(void)argv;
	line = NULL;
	cmd = NULL;
	signal(SIGQUIT, SIG_IGN);
	//signal(SIGINT, signal_handler);
	path = get_path(env);
	if (!path)
		exit(1);
	link_env = ft_lst_env(env);
	new_env = ft_env_from_lst(link_env);
	write(1, "$_sluty-shell_> ", ft_strlen("$_sluty-shell_> "));
	while (get_next_line(0, &line) > 0)
    {
	//	token = ft_lst_create_token(0, 0);
	//	lexer_build(line, &token);
		//parser_shell(token);
		cmd = ft_split_set(line, " ");
		free(line);
		line = NULL;
		if (!cmd)
		{
			free_array(path);
			exit(1);
		}
		if (cmd[0] && !ft_strncmp(cmd[0], "cd", 2))
		{
			if (cmd[1])
				built_in_cd(cmd[1]);
		}
		else if (cmd[0] && !ft_strncmp(cmd[0], "echo", 4))
				built_in_echo(cmd, link_env);
		else if (cmd[0] && !ft_strncmp(cmd[0], "env", 3))
				lst_print(link_env, 1);
		else if (cmd[0] && !ft_strncmp(cmd[0], "export", 6))
		{
			if (cmd[1])
				lst_set_var(&link_env, cmd[1]);
		}
		else if (cmd[0] && !ft_strncmp(cmd[0], "unset", 5))
		{
			if (cmd[1])
				lst_remove_var(&link_env, cmd[1]);
		}
		else if (!access(cmd[0], F_OK))
			exec_cmd(cmd, new_env);
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
					exec_cmd(cmd, new_env);
					break;
				}
				free(bin);
				bin = NULL;
			}
			if (!bin)
				write(1, "command not found\n", 18);
		}
		free_array(cmd);
		free_array(new_env);
		new_env = ft_env_from_lst(link_env);
		write(1, "$_sluty-shell_> ", ft_strlen("$_sluty-shell_> "));
	}
	free_array(path);
	ft_free_linkedlist(&link_env);
	return (0);
}

