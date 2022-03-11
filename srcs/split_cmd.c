/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:17:57 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 11:59:53 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_index_pipe(char *rdl)
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
		if (rdl[i] == '|' && quote == 0 && dquote == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	add_next_cmd(char *rdl, t_cmd **cmd, int *ip)
{
	char	*tmp;

	*ip = get_index_pipe(rdl);
	(*cmd)->next = 0;
	if (*ip > 0)
		tmp = ft_strndup(rdl, *ip);
	else
		tmp = ft_strdup(rdl);
	if (!tmp)
		return (1);	
	(*cmd)->str = ft_strtrim(tmp, " \t");
	free(tmp);
	if (!((*cmd)->str))
		return (1);
	return (0);
}

int	split_cmd(char *rdl, t_cmd **cmd)
{
	t_cmd	*tmp;
	int		is_pipe;
	
	tmp = (*cmd);
	if (add_next_cmd(rdl, &tmp, &is_pipe))
		return (1);
	while (is_pipe > 0)
	{
		tmp->next = (t_cmd *)malloc(sizeof(t_cmd));
		if (!tmp->next)
			return (1);
		tmp = tmp->next;
		ft_memset(tmp, 0, sizeof(tmp));
		rdl = rdl + is_pipe + 1;
		if (add_next_cmd(rdl, &tmp, &is_pipe))
			return (1);
	}
	return (0);
}

int	nbr_cmd(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd)
	{
		cmd = cmd->next;
		i++;
	}
	return (i);
}
