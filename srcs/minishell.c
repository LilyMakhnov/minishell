/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:25:06 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 15:44:47 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

int	end_quotes(char *str, int i, int quotes)
{
	if (((str)[i + 1] == '\"' && quotes == 2)
		|| ((str)[i + 1] == '\'' && quotes == 1))
		return (1);
	return (0);	
}

int remove_first_quotes(int *i, int *quotes, char **str)
{
	char *ret;
	int j;

	while((*str)[*i] && (*str)[*i] != '\"' && (*str)[*i] != '\'')
		(*i)++;
	if ((int)ft_strlen(*str) <= *i)
		return (0);	
	ft_update_quotes(*str, *i, quotes);
	while ((*str)[*i] && (*str)[*i + 1])
	{
		if (end_quotes(*str, *i, *quotes))
			break;
		(*str)[*i] = (*str)[(*i) + 1];
		(*i)++;
	}
	j = (*i) - 1;
	while ((*str)[++j + 2])
		(*str)[j] = (*str)[j + 2];
	(*str)[j] = 0;
	ret = ft_strdup(*str);
	if (!ret)
		return (free(*str), 1);
	free(*str);
	*str = ret;
	return (0);
}

int remove_quotes(char **str)
{
	int prev;
	int i;
	int quotes;

	prev = -1;
	i = 0;
	while (prev != i)
	{
		prev = i;
		if (remove_first_quotes(&i, &quotes, str))
			return (1);
	}
	return (0);
}

int	rm_quotes_cmd_token(t_cmd *cmd)
{
	t_token	*token;

	while (cmd)
	{
		token = cmd->token;
		while (token)
		{
		//	if (token->type == ARG)
		//	{
				if (remove_quotes(&token->data))
					return (1);
		//	}	
			token = token->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	init_arg(t_cmd *cmd)
{
	t_token	*token;
	int		i;

	token = cmd->token;
	i = 0;
	while (token)
	{
		if (token->type == ARG)
		{
			cmd->arg[i] = token->data;
			i++;
		}
		token = token->next;
	}
	cmd->arg[i] = NULL;
}

int	nbr_arg_cmd(t_cmd *cmd)
{
	t_token	*token;
	int		len;

	len = 0;
	token = cmd->token;
	while (token)
	{
		if (token->type == ARG)
			len++;
		token = token->next;
	}
	return (len);
}

int	organise_arg(t_cmd *cmd)
{
	int		len_arg;
	char	**new;

	while (cmd)
	{
		len_arg = nbr_arg_cmd(cmd);
		new = malloc(sizeof(char *) * (len_arg + 1));
		if (!new)
			return (1);
		cmd->arg = new;
		init_arg(cmd);
		cmd = cmd->next;
	}
	return (0);
}

void	close_fd_all(t_cmd **cmdl)
{
	t_cmd	*cur;

	cur = *cmdl;
	while (cur)
	{
		if (cur->fd_in != 0)
			close(cur->fd_in);
		if (cur->fd_out != 1)
			close(cur->fd_out);
		cur = cur->next;
	}
}

int	ft_is_builtin(char *str)
{
	if (str == NULL)
		return (0);
	else if (!ft_strncmp("exit", str, 4))
		return (1);
	else if (!ft_strncmp("cd", str, 2))
		return (1);
	else if (!ft_strncmp("echo", str, 4))
		return (1);
	else if (!ft_strncmp("env", str, 3))
		return (1);
	else if (!ft_strncmp("pwd", str, 3))
		return (1);
	else if (!ft_strncmp("export", str, 6))
		return (1);
	else if (!ft_strncmp("unset", str, 5))
		return (1);
	else
		return (0);
}

int	wait_pid(t_cmd **cmdl, pid_t *pid)
{
	t_cmd	*cur;
	int				len;
	int				i;

	i = 0;
	cur = *cmdl;
	len = nbr_cmd(cur);
	if (len == 1 && ft_is_builtin((*cmdl)->arg[0]))
		return (0);
	while (i < len)
	{
		waitpid(pid[i], &g_exit_status, 0);
		i++;
	}
	return (0);
}

void	exec_built_in_fd(t_cmd *cmd, t_env *env, int *pid)
{
	char **arg;

	arg = cmd->arg;
	if (arg[0] && !ft_strncmp(arg[0], "cd", 2))
	{
		if (arg[1])
			built_in_cd(arg[1]);
	}
	else if (arg[0] && !ft_strncmp(arg[0], "echo", 4)) 
		built_in_echo(arg, cmd->fd_out, env);
	else if (arg[0] && !ft_strncmp(arg[0], "env", 3))
		lst_print(env, cmd->fd_out);
	else if (arg[0] && !ft_strncmp(arg[0], "pwd", 3))
		built_in_pwd(cmd->fd_out);	
	else if (arg[0] && !ft_strncmp(arg[0], "exit", 4))
		built_in_exit(&cmd, env, pid);
	else if (arg[0] && !ft_strncmp(arg[0], "export", 6))
	{
		if (arg[1])
			lst_set_var(&env, arg[1]);
	}
	else if (arg[0] && !ft_strncmp(arg[0], "unset", 5))
	{
		if (arg[1])
			lst_remove_var(&env, arg[1]);
	}
}

void	exec_built_in(t_cmd *cmd, t_env *env, int *pid)
{
	char **arg;

	arg = cmd->arg;
	if (arg[0] && !ft_strncmp(arg[0], "cd", 2))
	{
		if (arg[1])
			built_in_cd(arg[1]);
	}
	else if (arg[0] && !ft_strncmp(arg[0], "echo", 4)) 
		built_in_echo(arg, 1, env);
	else if (arg[0] && !ft_strncmp(arg[0], "env", 3))
		lst_print(env, 1);
	else if (arg[0] && !ft_strncmp(arg[0], "pwd", 3))
		built_in_pwd(1);		
	else if (arg[0] && !ft_strncmp(arg[0], "exit", 4))
		built_in_exit(&cmd, env, pid);
	else if (arg[0] && !ft_strncmp(arg[0], "export", 6))
	{
		if (arg[1])
			lst_set_var(&env, arg[1]);
	}
	else if (arg[0] && !ft_strncmp(arg[0], "unset", 5))
	{
		if (arg[1])
			lst_remove_var(&env, arg[1]);
	}
}

int	find_path(t_cmd *cmd, char **env)
{
	int i;
	char **path;
	char	*bin;

	i = -1;
	path = get_path(env);
	if (!path)
		return (1);
	while (path[++i])
	{
		bin = ft_strtrijoin(path[i], "/", cmd->arg[0]);
		if (!bin)
			return (free_array(path), 1);		
		if (!access(bin, F_OK))
			execve(bin, cmd->arg, env);
		free(bin);
		bin = NULL;
	}
	if (!bin)
		write(1, "command not found\n", 18);
	free_array(path);
	return (0);
}

int	go_to_exec(t_env *link_env, t_cmd *cm, pid_t *pid)
{
	char	**new_env;
	char	**cmd;
	int 	i;

	i = 0;
	cmd = cm->arg;
	if (ft_is_builtin(cmd[0]))
		exec_built_in(cm, link_env, pid);
	else if (!access(cmd[0], F_OK))
	{
		new_env = ft_env_from_lst(link_env);
		execve(cmd[0], cmd, new_env);
		free_array(new_env);
	}
	else if (cmd[0])
	{
		new_env = ft_env_from_lst(link_env);
		if (find_path(cm, new_env))
			i = 1;
		free_array(new_env);	
	}
	close_fd_all(&cm);
	free(pid);
	ft_free_cmd(&cm);
	ft_free_env(&link_env);
	close(0);
	close(1);
	exit(i);
	return (0);
}

int	go_to_pipe(int **nfd, int i, t_cmd *cur)
{
	if (cur->next)
	{
		nfd[i] = malloc(sizeof(int) * (2));
		if (nfd[i] == NULL)
			return (1);
		if (pipe(nfd[i]) == -1)
			return (1);
		if (i == 0)
			cur->fd_in = 0;
		else if (i > 0)
			cur->fd_in = nfd[i - 1][0];
		cur->fd_out = nfd[i][1];
	}
	else
	{
		if (i == 0)
			cur->fd_in = 0;
		else
			cur->fd_in = nfd[i - 1][0];
		cur->fd_out = 1;
	}
	return (0);
}

void	free_nfd(int **nfd)
{
	int i;

	i = 0;
	while (nfd[i])
	{
		free(nfd[i]);
		i++;
	}
	free(nfd);
}

int	init_fd_pipe(t_cmd **cmd)
{
	t_cmd	*cur;
	int				**nfd;
	int				i;
	int				ret;

	i = 0;
	cur = *cmd;
	nfd = malloc(sizeof(int *) * (nbr_cmd(cur)));
	if (nfd == NULL)
		return (1);
	nfd[nbr_cmd(cur) - 1] = NULL;
	while (cur)
	{
		ret = go_to_pipe(nfd, i, cur);
		if (ret != 0)
		{
			free_nfd(nfd);
			return (ret);
		}
		cur = cur->next;
		i++;
	}
	free_nfd(nfd);
	return (0);
}

int	redirect_file_in(t_cmd **cmd, t_token *token, t_e_token type)
{
	if (type == OPEN_FILE)
	{
		if ((*cmd)->fd_in != 0)
			close((*cmd)->fd_in);
		(*cmd)->fd_in = open(token->data, O_RDONLY);
		if ((*cmd)->fd_in == -1)
		{
			write(1, "minishell: ", 11);
			perror(token->data);
			return (-1);
		}
	}
	else if (type == LIMITOR)
	{
		if ((*cmd)->fd_in != 0)
			close((*cmd)->fd_in);
		//(*cmd)->fd_in = create_heredoc_fd(cmd, &token);
		//if ((*cmd)->fd_in == -1)
		//	return (-1);
	}
	return (0);
}

int	redirect_file_out(t_cmd **cmd, t_token *token, t_e_token type)
{
	if (type == EXIT_FILE)
	{
		if ((*cmd)->fd_out != 1)
			close((*cmd)->fd_out);
		(*cmd)->fd_out = open(token->data, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if ((*cmd)->fd_out == -1)
			return (-1);
	}
	else if (type == EXIT_FILE_RET)
	{
		if ((*cmd)->fd_out != 1)
			close((*cmd)->fd_out);
		(*cmd)->fd_out = open(token->data, O_CREAT | O_RDWR | O_APPEND, 0644);
		if ((*cmd)->fd_out == -1)
			return (-1);
	}
	return (0);
}

int	open_fd(t_cmd **cmd)
{
	t_token	*token;

	token = (*cmd)->token;
	while (token)
	{
		if (token->type == OPEN_FILE || token->type == LIMITOR)
			if (redirect_file_in(cmd, token, token->type) == -1)
				return (-1);
		if (token->type == EXIT_FILE || token->type == EXIT_FILE_RET)
			if (redirect_file_out(cmd, token, token->type) == -1)
				return (-1);
		token = token->next;
	}
	return (0);
}

int	ft_execve_fct(t_cmd **cmdl, t_cmd **first, t_env *env, pid_t *pid)
{
	dup2((*cmdl)->fd_in, 0);
	dup2((*cmdl)->fd_out, 1);
	close_fd_all(first);
	go_to_exec(env, *cmdl, pid);

	return (0);
}

int	multi_fork(pid_t *pid, int i, t_cmd **cmdl, t_env *env)
{
	t_cmd *cur;

	cur = get_i_cmd(*cmdl, i);
	pid[i] = fork();
	if (pid[i] == 0)
		ft_execve_fct(&cur, cmdl, env, pid);
	if ((cur)->fd_in != 0)
		close((cur)->fd_in);
	if ((cur)->fd_out != 1)
		close((cur)->fd_out);	
	return (0);
}

int	forking(t_cmd **cmdl, pid_t *pid, t_env *env)
{
	int				len;
	int				i;
	t_cmd	*cur;

	i = 0;
	cur = *cmdl;
	len = nbr_cmd(cur);
	while (cur)
	{
		open_fd(&cur);
		cur = cur->next;
	}
	cur = *cmdl;
	if (len == 1 && ft_is_builtin(cur->arg[0]))
	{
		exec_built_in_fd(*cmdl, env, pid);
		return (0);
	}
	while (i < len)
	{
		multi_fork(pid, i, cmdl, env);
		cur = cur->next;
		i++;
	}
	return (0);
}

int	go_to_fork(t_cmd **cmd, t_env *env)
{
	pid_t			*pid;
	t_cmd *cur;

	cur = *cmd;
	pid = malloc(sizeof(pid_t) * nbr_cmd(cur));
	if (pid == NULL)
		return (1);
	forking(cmd, pid, env);
	wait_pid(cmd, pid);
	free(pid);
	if (!ft_strncmp((*cmd)->arg[0], "exit", 4))
	{
		close_fd_all(cmd);
		ft_free_cmd(cmd);
		ft_free_env(&env);
		exit(0);
	}
	return (0);
}

int	main(int argc, char **argv, char **env) // too many lines
{
	char	*rdl;
	t_cmd	*cmd;
	t_env	*llenv;

	(void)argc;
	(void)argv;
	g_exit_status = 0;
	signal(SIGQUIT, SIG_IGN);
	llenv = ft_lst_env(env);
	if (!llenv)
	{
		write(2, "Error malloc\n", ft_strlen("Error malloc\n"));
		return (1);
	}
	while (1)
	{
		rdl = readline("$_minishell_> ");
		add_history(rdl);
		while (is_char_in_set(*rdl, " \t"))
			rdl++;
		if (rdl[0] && !check_syntax_pipe(rdl) && !is_open_quote(rdl))
		{
			cmd = (t_cmd *)malloc(sizeof(t_cmd));
			if (!cmd)
				return (1);
			ft_memset(cmd, 0, sizeof(cmd));
			if (split_cmd(rdl, &cmd))
			{
				ft_free_cmd(&cmd);
				free(rdl);
				return (1);
			}
			free(rdl);
			cmd_token(&cmd);
			if (!check_token_cmd(cmd))
			{
				expand_cmd_token(cmd, llenv);

				rm_quotes_cmd_token(cmd); //to protect
				organise_arg(cmd);
	//			print_cmd_token(cmd);
				init_fd_pipe(&cmd);
				go_to_fork(&cmd, llenv);

			}
			close_fd_all(&cmd);
			ft_free_cmd(&cmd);			
		}		
	}
	return (0);
}
