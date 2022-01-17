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
			if (!path)
				return (NULL);
			ret = ft_split(path, ':');
			free(path);
			if (!ret)
				return (NULL);
			return (ret);
		}
	return (NULL);
}

void	lst_print(t_env *env, int fd)
{
	while (env)
	{
		write(fd, env->value, ft_strlen(env->value));
		if (env->value)
			write(fd, "\n", 1);
		env = env->next;
	}
}

void	lst_remove_var(t_env **env, char *var)
{
	size_t	i;
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	if(!ft_strncmp(tmp->value, var, ft_strlen(var)) 
				&& tmp->value[ft_strlen(var)] == '=')
	{
		*env = tmp->next;
		free(tmp->value);
		free(tmp);
	}
	else
	{
		prev = tmp;
		tmp = tmp->next;
	}
	while (tmp)
	{
		if(!ft_strncmp(tmp->value, var, ft_strlen(var)) 
				&& tmp->value[ft_strlen(var)] == '=')
		{
			prev->next = tmp->next;
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	lst_set_var(t_env **env, char *var)
{
	size_t i;
	t_env *tmp;

	tmp = *env;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if(!ft_strncmp(tmp->value, var, i + 1))
		{
			free(tmp->value);
			tmp->value = ft_strdup(var);
			return ;
		}
		tmp = tmp->next;
	}
	tmp = ft_lst_create(var);
	ft_lst_pushback(env, tmp);
}

char	*lst_get_var(t_env *env, char *var)
{
	char *ret;

	while (env)
	{
		if(!ft_strncmp(env->value, var, ft_strlen(var))
						&& env->value[ft_strlen(var)] == '=')
		{
			ret = ft_strdup(env->value + ft_strlen(var) + 1);
			if (!ret)
				return (NULL);
			return (ret);
		}
		env = env->next;
	}
	return (NULL);
}

char	*get_var(char **env, char *var)
{
	int i;
	char *ret;

	i = -1;
	while (env[++i])
	{
		if(!ft_strncmp(env[i], var, ft_strlen(var))
						&& env[i][ft_strlen(var)] == '=')
		{
			ret = ft_strdup(env[i] + ft_strlen(var) + 1);
			if (!ret)
				return (NULL);
			return (ret);
		}
	}
	return (NULL);
}

char	*ft_strtrijoin(char *str1, char *str2, char *str3)
{
	char *bin;

	bin = malloc(ft_strlen(str1) + ft_strlen(str2) + ft_strlen(str3) + 1);
	if (!bin)
		return (NULL);
	//bin[0] = 0;
	ft_strlcpy(bin, str1, ft_strlen(str1) + 1);
	ft_strlcat(bin, str2, ft_strlen(bin) + ft_strlen(str2) + 1);
	ft_strlcat(bin, str3, ft_strlen(bin) + ft_strlen(str3) + 1);
	return (bin);
}

void	built_in_cd(char *path)
{
	if (chdir(path) == -1) {
		perror("chdir()");
	}
}

int	print_var(char *str, t_env *env)
{
	while (env)
	{
		if(!ft_strncmp(env->value, str, ft_strlen(str))
						&& env->value[ft_strlen(str)] == '=')
		{
			write(1, env->value + ft_strlen(str) + 1,
				 ft_strlen(env->value + ft_strlen(str) + 1));
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int parse_var(char *cmd, int *j, t_env *env)
{
	char **var;
	int	l;
	int t;

	t = 0;
	var = ft_split(cmd + *j, '$');
	l = -1;
	while (var[++l])	
	{
		t = print_var(var[l], env);					
		(*j) +=  ft_strlen(var[l]);
		if (l > 0)
			(*j)++;
	}
	l = -1;
	while (var[++l])
		free(var[l]);
	free(var);		
	return (t);
}

void	built_in_echo(char **cmd, t_env *env)
{
	int i;
	int j;
	int t;

	i = 0;
	if (!ft_strncmp(cmd[1], "-n", 2))
		i++;
	while (cmd[++i])
	{
		j = 0;
		t = 0;
		while(cmd[i][j])
		{
			if (cmd[i][j] != '$')
			{
				write(1, &cmd[i][j], 1);
				t++;
			}
			else
				t += parse_var(cmd[i], &j, env);
			j++;
		}
		if (cmd[i + 1] && t > 0)
			write(1, " ", 1);
	}
	if (ft_strncmp(cmd[1], "-n", 2))
		write (1, "\n", 1);
}

int	main(int argc, char **argv, char **env)
{
	t_env	*link_env;
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
	write(1, "$_sluty-shell_> ", ft_strlen("$_sluty-shell_> "));
	while (get_next_line(0, &line) > 0)
    {
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
		else if (cmd[0] && !ft_strncmp(cmd[0], "expand", 6))
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
		write(1, "$_sluty-shell_> ", ft_strlen("$_sluty-shell_> "));
	}
	free_array(path);
	ft_free_linkedlist(&link_env);
	return (0);
}

