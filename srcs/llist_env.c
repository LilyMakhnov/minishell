/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:08:59 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 10:53:41 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_envlen(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		env = env->next;
		i++;
	}
	return (i);
}

char	**ft_env_from_lst(t_env *llenv)
{
	int		i;
	char	**env;

	env = malloc(sizeof(char *) * (ft_envlen(llenv) + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (llenv)
	{
		env[i] = ft_strdup(llenv->value);
		if (!env[i])
		{
			while (i)
				free(env[i-- - 1]);
			return (free(env), NULL);
		}
		llenv = llenv->next;
		i++;
	}
	env[i] = 0;
	return (env);
}

t_env	*ft_lst_env(char **env)
{
	t_env	*first;
	t_env	*tmp;
	int		i;

	if (env[0])
	{
		first = ft_lst_create(env[0]);
		if (!first)
			return (NULL);
	}
	else
		return (NULL);
	tmp = first;
	i = 0;
	while (tmp && env[++i])
	{
		tmp->next = ft_lst_create(env[i]);
		if (!(tmp->next))
			return (ft_free_env(&first), NULL);
		tmp = tmp->next;
	}
	return (first);
}

void	lst_print(t_env *env, int fd)
{
	while (env)
	{
		write(fd, env->value, ft_strlen(env->value));
		if (env->value)
			write(fd, "\n", 1);
		env = env->next;
	}
}
