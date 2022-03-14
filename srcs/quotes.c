/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:58:26 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 11:54:15 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_update_quotes(char *str, int i, int *quotes)
{
	int	j;

	j = 0;
	*quotes = 0;
	while (str[j] && j <= i)
	{
		if (str[j] == '\'' && *quotes == 0)
			*quotes = 1;
		else if (str[j] == '\"' && *quotes == 0)
			*quotes = 2;
		else if (str[j] == '\'' && *quotes == 1)
			*quotes = 0;
		else if (str[j] == '\"' && *quotes == 2)
			*quotes = 0;	
		j++;
	}
}

int	is_open_quote(char *rdl)
{
	int	i;
	int	dquote;
	int	quote;

	dquote = 0;
	quote = 0;
	i = 0;
	while (rdl[i])
	{
		if (rdl[i] == '\"' && dquote == 0 && quote == 0)
			dquote = 1;
		else if (rdl[i] == '\"' && dquote == 1)
			dquote = 0;
		if (rdl[i] == '\'' && quote == 0 && dquote == 0)
			quote = 1;
		else if (rdl[i] == '\'' && quote == 1)
			quote = 0;
		i++;
	}
	if (quote + dquote > 0)
		write(2, "syntax error quotes\n", 20);
	return (quote + dquote);
}

int	cmd_token(t_cmd **cmd)
{
	t_cmd	*tmp;

	tmp = *cmd;
	while (tmp)
	{
		tmp->token = ft_lst_create_token("");
		if (!tmp->token || lexer_build(tmp->str, &tmp->token))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
