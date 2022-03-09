/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   llist_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:13:06 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 10:14:42 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_env	*ft_lst_create(char *value)
{
	t_env	*element;

	element = (t_env *)malloc(sizeof(t_env));
	if (!element)
		return (NULL);
	element->value = ft_strdup(value);
	if (!(element->value))
		return (free(element), NULL);
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
