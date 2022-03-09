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

int	main(int argc, char **argv, char **env) // too many lines
{
	char	*rdl;
	t_cmd	*cmd;
	t_env	*llenv;

	(void)argc;
	(void)argv;
	llenv = ft_lst_env(env);
	g_exit_status = 0;
	signal(SIGQUIT, SIG_IGN);
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
				return (1);
			}
			cmd_token(&cmd);
			if (!check_token_cmd(cmd))
			{
				expand_cmd_token(cmd, llenv);
				rm_quotes_cmd_token(cmd); //to protect
				organise_arg(cmd);
				print_cmd_token(cmd);
			}
			ft_free_cmd(&cmd);
		}			
	}
	return (0);
}
