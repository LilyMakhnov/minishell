/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:46:29 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 10:47:56 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_token(t_token *token)
{
	int			operator;
	t_e_token	prev;

	prev = NONE;
	if (token->type != ARG)
		return (unexpected_token(token->data), 1);
	operator = 0;
	while (token)
	{
		if (is_type_operator(token->type))
			operator++;
		else
		{
			operator = 0;
			token->type = upgrade_type(prev, token->type);
		}
		if (operator > 1)
			return (unexpected_token(token->data), 1);
		if (!token->next && is_type_operator(token->type))
			return (unexpected_token("\n"), 1);
		prev = token->type;
		token = token->next;
	}
	return (0);
}

int	check_token_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		if (check_token(cmd->token))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
