/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 09:48:53 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 14:13:44 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var(char **env, char *var)
{
	int		i;
	char	*ret;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], var, ft_strlen(var))
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

int	lst_get_var(t_env *env, char *var, char **value)
{
	while (env)
	{
		if (!ft_strncmp(env->value, var, ft_strlen(var))
			&& env->value[ft_strlen(var)] == '=')
		{
			(*value) = ft_strdup(env->value + ft_strlen(var) + 1);
			if (!(*value))
				return (1);
			return (0);
		}
		env = env->next;
	}
	(*value) = NULL;
	return (0);
}

void	lst_set_var(t_env **env, char *var)
{
	size_t	i;
	t_env	*tmp;

	tmp = *env;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if (!ft_strncmp(tmp->value, var, i + 1))
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

void	lst_remove_var(t_env **env, char *var) // too many lines a revoir
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	if (!ft_strncmp(tmp->value, var, ft_strlen(var))
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
		if (!ft_strncmp(tmp->value, var, ft_strlen(var))
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

char	**get_path(char **env)
{
	int		i;
	char	*path;
	char	**ret;

	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5)) // line 141 Multiple instructions in single line control structure
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
	}
	return (NULL);
}
