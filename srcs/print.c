/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 09:55:30 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 11:56:06 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd_token(t_cmd *cmd)
{
	t_token	*token;
	int i;

	while (cmd)
	{
		write(1, "cmd :\n", 6);
		write(1, cmd->str, ft_strlen(cmd->str));
		write(1, "\n", 1);
		write(1, "token :\n", 8);
		token = cmd->token;
		while (token)
		{
			write(1, token->data, ft_strlen(token->data));
			write(1, "\n", 1);
			printf("type :%i\n", token->type);
			token = token->next;
		}
		i = -1;
		write(1, "arg :\n", 6);
		while (cmd->arg[++i])
		{
		write(1, cmd->arg[i], ft_strlen(cmd->arg[i]));
		write(1, "\n", 1);
		}
		cmd = cmd->next;
	}
}

void	print_token(t_token *token)
{
	write(1, "token :\n", 8);
	while (token)
	{
		if (token->data)
			write(1, token->data, ft_strlen(token->data));
		write(1, "\n", 1);
		token = token->next;
	}
}

void	print_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		write(1, "cmd:\n", 5);
		if (cmd->str)
			write(1, cmd->str, ft_strlen(cmd->str));
		write(1, "\n", 1);
		cmd = cmd->next;
	}
}
