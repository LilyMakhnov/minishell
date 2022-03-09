/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:00:21 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 14:07:43 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	lexer_general_std(char a, int *state, t_token **token)
{
	if (a == '\'')
		*state = 1;
	else if (a == '\"')
		*state = 2;
	(*token)->data = ft_straddchar((*token)->data, a);
	if (!((*token)->data))
		return (1);
	(*token)->type = ARG;
	return (0);
}

int	lexer_general_op(char *str, int *i, t_token **token)
{
	if ((*token)->data && ft_strlen((*token)->data) > 0)
	{
		if (new_token_next(token))
			return (1);
	}
	(*token)->data = ft_straddchar((*token)->data, str[*i]);
	if (!((*token)->data))
		return (1);
	(*token)->type = get_type(str, *i);
	if (is_char_in_set(str[*i], "<>"))
	{
		if (str[*i + 1] && str[*i + 1] == str[*i])
		{
			(*token)->data = ft_straddchar((*token)->data, str[*i]);
			if (!((*token)->data))
				return (1);
			(*i)++;
		}
	}
	if (str[*i + 1])
	{			
		if (new_token_next(token))
			return (1);
	}
	return (0);
}

int	lexer_general(char *str, int *i, t_token **token, int *state)
{
	if (is_char_in_set(str[*i], " \t"))
	{
		if ((*token)->data && ft_strlen((*token)->data) > 0)
		{
			if (new_token_next(token))
				return (1);
		}
	}
	else if (is_char_in_set(str[*i], "<>|"))
	{
		if (lexer_general_op(str, i, token))
			return (1);
	}
	else
		if (lexer_general_std(str[*i], state, token))
			return (1);
	return (0);
}

int	lexer_quote(char *line, int i, t_token **token, int *state)
{
	(*token)->data = ft_straddchar((*token)->data, line[i]);
	if (!((*token)->data))
		return (1);
	if ((line[i] == '\"' && *state == 2) || (line[i] == '\'' && *state == 1))
		*state = 0;
	return (0);
}

int	lexer_build(char *cmd, t_token **src)
{	
	t_token	*token;
	int		i;
	int		state;
	char	*line;

	state = 0;
	token = *src;
	i = -1;
	line = ft_strtrim(cmd, " \t");
	if (!line)
		return (1);
	while (line && line[++i])
	{
		if (state == 0)
		{
			if (lexer_general(line, &i, &token, &state))
				return (free(line), 1);
		}
		else if (state == 1 || state == 2)
		{
			if (lexer_quote(line, i, &token, &state))
				return (free(line), 1);
		}	
	}
	return (free(line), 0);
}

t_e_token	upgrade_type(t_e_token prev, t_e_token curr)
{
	if (prev == FILE_IN)
		return (OPEN_FILE);
	if (prev == HERE_DOC)
		return (LIMITOR);
	if (prev == FILE_OUT)
		return (EXIT_FILE);
	if (prev == FILE_OUT_SUR)
		return (EXIT_FILE_RET);
	return (curr);
}

int	is_type_operator(t_e_token type)
{
	if (type == FILE_IN || type == HERE_DOC
		|| type == FILE_OUT || type == FILE_OUT_SUR)
		return (1);
	else
		return (0);
}
