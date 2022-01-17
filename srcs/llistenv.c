#include <minishell.h>

t_env	*ft_lst_env(char **env)
{
	t_env	*first;
	t_env	*tmp;
	int 	i;

	if (env[0])
		first = ft_lst_create(env[0]);
	else
		return (0);
	tmp = first;
	i = 0;
	while (first && env[++i])
	{
		tmp->next = ft_lst_create(env[i]);
		tmp = tmp->next;
	}
	return (first);
}

t_env	*ft_lst_create(char *value)
{
	t_env	*element;

	element = (t_env *)malloc(sizeof(t_env));
	if (!element)
		return (NULL);
	element->value = ft_strdup(value);
	element->next = NULL;
	return (element);
}

t_env	*ft_lst_last(t_env *lst)
{
	t_env	*end_lst;

	end_lst = lst;
	while (lst)
	{
		end_lst = lst;
		lst = lst->next;
	}
	return (end_lst);
}

void	ft_lst_pushback(t_env **liste, t_env *element)
{
	t_env	*last;

	if (!element)
		return ;
	if (!(*liste))
		*liste = element;
	else
	{
		last = (*liste);
		while (last->next)
			last = last->next;
		last->next = element;
	}
}

void	ft_free_linkedlist(t_env **liste)
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