/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:55:26 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 14:06:42 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_var(char **str, int i, t_env *env)
{
	char	*value;
	char	*start;
	char	*res;

	start = ft_strndup(*str, i);
	if (!start)
		return (1);
	if ((*str)[i + 1] && (*str)[i + 1] == '?')
	{
		value = ft_itoa(g_exit_status);
		if (!value)
			return (free(start), 1);
	}
	else if (get_value_str(*str, i, &value, env))
		return (free(start), 1);
	res = ft_strtrijoin(start, value,
			(*str) + i + 1 + ft_var_name_len((*str) + i + 1));
	free_three_str(*str, start, value);
	if (!res)
		return (1);
	*str = res;
	return (0);
}

int	expand_str(char **str, t_env *env)
{
	int	i;
	int	quotes;

	i = -1;
	while ((*str)[++i])
	{
		ft_update_quotes(*str, i, &quotes);
		if ((*str)[i] == '$' && quotes != 1)
		{
			if (expand_var(str, i, env))
				return (1);
			if (i >= (int)ft_strlen(*str))
				return (0);
		}
	}
	return (0);
}

int	expand_cmd_token(t_cmd *cmd, t_env *env)
{
	t_token	*token;

	while (cmd)
	{
		token = cmd->token;
		while (token)
		{
			if (token->type == 1)
			{
				if (expand_str(&token->data, env))
					return (1);
			}
			token = token->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	get_value_str(char *str, int i, char **value, t_env *env)
{
	char	*name;

	name = ft_strndup((str) + i + 1, ft_var_name_len((str) + i + 1));
	if (!name)
		return (1);
	if (lst_get_var(env, name, value))
		return (free(name), 1);
	free(name);
	if (!(*value))
	{
		*value = ft_strdup("");
		if (!(*value))
			return (1);
	}
	return (0);
}

int	ft_var_name_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i] == '?')
		i++;
	return (i);
}

