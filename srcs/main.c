/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:15:32 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 10:16:15 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token **token)
{
	t_token	*tok;

	while (*token)
	{
		tok = (*token)->next;
		if ((*token)->data)
			free((*token)->data);
		free(*token);
		(*token) = tok;
	}
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	if (cmd)
	{
		while (*cmd)
		{
			tmp = (*cmd)->next;
			if ((*cmd)->str)
				free((*cmd)->str);
		//	if ((*cmd)->arg)
		//		free((*cmd)->argv);
			if ((*cmd)->token)
				free_token((*cmd)->token);
		//	if ((*cmd)->name_file)
		//		free((*cmd)->name_file);
			free((*cmd));
			*cmd = tmp;
		}
	}
}

void	go_to_exec(t_cmd **cmd, char *str)
{
	int	res;

	res = pars(str, cmd);
	if (res == 50)
	{
		free_all_env_str_ret_malloc_error(cmd_line, str);
		exit (1);
	}
	if (res == 0)
	{
		if (str && *cmd)
		{
			res = ft_exec(cmd_line);
			if (res != 0)
			{
				free_all_error(cmd_line, str, res);
				exit (1);
			}
		}
	}
}

int	main(int argc, char **argv, char **env) // too many lines
{
	char	*rdl;
	t_cmd	*cmd;

	cmd_line = NULL;
	g_exit_status = 0;
//	ft_init_t_env(envp);
//	signal(SIGINT, signal_cmd);
	signal(SIGQUIT, SIG_IGN);
//	if (!argc && !av)
//		return (0);
	while (1)
	{
		rdl = readline("sluty_shell$> ");
		add_history(rdl);
//		signal(SIGINT, signal_cmd);
		signal(SIGQUIT, SIG_IGN);
//		if (str == NULL)
//			return (print_exit_free_env_all(&cmd_line));
//		if (check_str(str) != 0)
//			error_str();
//		else
			go_to_exec(&cmd, rdl);
		if (rdl)
			free(rdl);
		free_cmd(&cmd);
	}
}
