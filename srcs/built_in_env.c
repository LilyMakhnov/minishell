#include "minishell.h"

char	*get_var(char **env, char *var)
{
	int i;
	char *ret;

	i = -1;
	while (env[++i])
	{
		if(!ft_strncmp(env[i], var, ft_strlen(var))
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

char	*lst_get_var(t_env *env, char *var)
{
	char *ret;

	while (env)
	{
		if(!ft_strncmp(env->value, var, ft_strlen(var))
						&& env->value[ft_strlen(var)] == '=')
		{
			ret = ft_strdup(env->value + ft_strlen(var) + 1);
			if (!ret)
				return (NULL);
			return (ret);
		}
		env = env->next;
	}
	return (NULL);
}

void	lst_set_var(t_env **env, char *var)
{
	size_t i;
	t_env *tmp;

	tmp = *env;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (tmp)
	{
		if(!ft_strncmp(tmp->value, var, i + 1))
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

void	lst_remove_var(t_env **env, char *var)
{
	size_t	i;
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	if(!ft_strncmp(tmp->value, var, ft_strlen(var)) 
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
		if(!ft_strncmp(tmp->value, var, ft_strlen(var)) 
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
	int i;
	char *path;
	char **ret;

	i = -1;
	while (env[++i])
		if(!ft_strncmp(env[i], "PATH=", 5))
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
	return (NULL);
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