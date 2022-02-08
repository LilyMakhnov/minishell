#include <minishell.h>

t_token	*ft_lst_create_token(char *data, int type)
{
	t_token	*element;

	element = (t_token *)malloc(sizeof(t_token));
	if (!element)
		return (NULL);
	element->data = ft_strdup(data);
	element->type = type;
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

void	lexer_general(char *str, int *i, t_token **token, int *state)
{
	if (is_char_in_set(str[*i], " \t<>;&|"))
	{
		if ((*token)->data && ft_strlen((*token)->data) > 0) 
		{
			(*token)->next = ft_lst_create_token(0, 0);
			*token = (*token)->next;
		}
		if (!is_char_in_set(str[*i], " \t"))
		{
			(*token)->data = ft_straddchar((*token)->data, str[*i]);
			(*token)->type = str[*i];
			if (is_char_in_set(str[*i], "<>"))
				if (str[*i + 1] && str[*i + 1] == str[*i])
				{
					(*token)->data = ft_straddchar((*token)->data, str[*i]);
					(*token)->type += str[(*i)++];
				}					
			(*token)->next = ft_lst_create_token(0, 0);
			*token = (*token)->next;
		}
	}
	else
	{
		if (str[*i] == '\'')
			*state = 1;
		else if (str[*i] == '\"')
			*state = 2;
		(*token)->data = ft_straddchar((*token)->data, str[*i]);
		(*token)->type = 1;
	}
}

void lexer_build(char* cmd, t_token **src)
{	
	t_token	*token;
	int		i;
	int		state;
	char	*line;
	
	state = 0;
	token = *src;
	i = 0;
	line = ft_strtrim(cmd, " \t");
	while (line && line[i])
	{
		if (state == 0)
			lexer_general(line, &i, &token, &state);
		else if (state == 1 || state == 2 ) 
		{
			token->data = ft_straddchar(token->data, line[i]);
			if ((line[i] == '\"' && state == 2)
				|| (line[i] == '\'' && state == 1))
				state = 0;	
		}	
		i++;
	}
	if (line)
		free(line);
}

