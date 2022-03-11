/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:50:59 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 11:58:25 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_three_str(char *str1, char *str2, char *str3)
{
	if (str1)
	{
		free(str1);
		str1 = NULL;
	}
	if (str2)
	{
		free(str2);
		str2 = NULL;
	}
	if (str3)
	{
		free(str3);
		str3 = NULL;
	}
}

void	ft_free_env(t_env **liste)
{
	t_env	*tmp;

	while (*liste)
	{
		tmp = (*liste)->next;
		free((*liste)->value);
		free(*liste);
		*liste = tmp;
	}
}

void	ft_free_token(t_token **token)
{
	t_token	*tmp;

	while (*token)
	{
		tmp = (*token)->next;
		if ((*token)->data)
		{
			free((*token)->data);
			(*token)->data = NULL;
		}
		free(*token);
		*token = tmp;
	}
}

void	ft_free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	while (*cmd)
	{
		tmp = (*cmd)->next;
		free((*cmd)->str);
		free((*cmd)->arg);
		ft_free_token(&(*cmd)->token);
		free(*cmd);
		*cmd = tmp;
	}
}

void	free_array(char **array)
{
	int i;

	i = 0;
	if (array)
	{
		while (array[i])
		{
			free(array[i]);
			array[i] = NULL;
			i++;
		}
	}
	if (array)
	{
		free(array);
		array = NULL;
	}
}
