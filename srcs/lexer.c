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
/*
enum type{
    NONE, //defaut set
    ARG, //word
    FILE_IN, //word == '<'
    HERE_DOC, // word == '<<'
    FILE_OUT, //word == '>'
    FILE_OUT_SUR, //word == '>>'
    OPEN_FILE, // word following '<'
    LIMITOR, // word following '<<'
    EXIT_FILE, // word followinf '>'
    EXIT_FILE_RET; // word following '>>'
}*/

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
	//(*token)->type = str[*i];
	(*token)->type = get_type(str, *i);
	if (is_char_in_set(str[*i], "<>"))
	{
		if (str[*i + 1] && str[*i + 1] == str[*i])
		{
			(*token)->data = ft_straddchar((*token)->data, str[*i]);
			if (!((*token)->data))
				return (1);
			(*i)++;
	//		(*token)->type += str[(*i)++];
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
