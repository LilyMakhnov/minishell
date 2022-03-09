/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 09:59:46 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 10:57:12 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_token	*ft_lst_create_token(char *data)
{
	t_token	*element;

	element = malloc(sizeof(t_token));
	if (!element || !data)
		return (NULL);
	element->data = ft_strdup(data);
	if (!(element->data))
		return (free(element), NULL);
	element->type = NONE;
	element->next = NULL;
	return (element);
}

int	is_char_in_set(char c, char *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

int	new_token_next(t_token **token)
{
	(*token)->next = ft_lst_create_token("");
	*token = (*token)->next;
	if (!(*token))
		return (1);
	return (0);
}

t_e_token	get_type(char *str, int i)
{
	if (str[i] == '>' && str[i + 1] && str[i + 1] == '>')
		return (FILE_OUT_SUR);
	if (str[i] == '>')
		return (FILE_OUT);
	if (str[i] == '<' && str[i + 1] && str[i + 1] == '<')
		return (HERE_DOC);
	if (str[i] == '<')
		return (FILE_IN);
	return (ARG);
}

void	unexpected_token(char *str)
{
	write(2, "syntax error near unexpected token `",
		ft_strlen("syntax error near unexpected token `"));
	if (ft_strlen(str) == 1 && str[0] == '\n')
		write(2, "newline", ft_strlen("newline"));
	else
		write(2, str, ft_strlen(str));
	write(2, "\'\n", 2);
	g_exit_status = 2;
}
