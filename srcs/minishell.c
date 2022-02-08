#include "minishell.h"

char	*ft_strndup(const char *src, size_t n)
{
	int		i;
	char	*dest;

	if (!src)
		return (NULL);
	i = 0;
	if (n < ft_strlen(src))
		dest = malloc(n + 1);
	else
		dest = malloc(ft_strlen(src) + 1);
	if (!dest)
		return (NULL);
	while (src[i] && --n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int check_pipe_err(char *str, int i)
{
	i++;
	while (str[i] && is_char_in_set(str[i], " \t"))
		i++;
	if (str[i] && str[i] == '|')
		return (1);
	return (0);	
}

int check_syntax_pipe(char *rdl)
{
	char *str;
	int ret;
	int i;

	i = 0;
	ret = 0;
	str = ft_strtrim(rdl, " \t\n");
	if (str[i] && is_char_in_set(str[i], "|<>")) 
		ret = str[i];
	while (str[i])
	{
		if (check_pipe_err(str + i, get_index_pipe(str + i)))
			ret = '|';
		i++;
	}
	if (is_char_in_set(str[i - 1], "|<>"))
		ret = str[i - 1];
	free(str);	
	if (ret > 0)
	{
		write(2, "syntax error ", 13);
		write(2, &ret, 1);
		write(2, "\n", 1);
	}
	return (ret);
}

void	ft_free_token(t_token **token)
{
	t_token	*tmp;

	while (*token)
	{
		tmp = (*token)->next;
		free((*token)->data);
		free(*token);
		*token = tmp;
	}
}

void	ft_free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	while (*cmd)
	{
		tmp = (*cmd)->next;
		free((*cmd)->cmd);
		ft_free_token(&(*cmd)->token);
		free(*cmd);
		*cmd = tmp;
	}
}

int	is_open_quote(char *rdl)
{
	int i;
	int dquote;
	int quote;

	dquote = 0;
	quote = 0;
	i = 0;
	while (rdl[i])
	{
		if (rdl[i] == '\"' && dquote == 0 && quote == 0)
			dquote = 1;
		else if (rdl[i] == '\"' && dquote == 1)
			dquote = 0;	
		if (rdl[i] == '\'' && quote == 0 && dquote == 0)
			quote = 1;
		else if (rdl[i] == '\'' && quote == 1)
			quote = 0;			
		i++;
	}
	if (quote + dquote > 0)
		write(2, "syntax error quotes\n", 20);
	return (quote + dquote);
} 

void	print_cmd_token(t_cmd *cmd)
{
	while (cmd)
	{
		write(1, "cmd :\n", 6);
		write(1, cmd->cmd, ft_strlen(cmd->cmd));
		write(1, "\n", 1);
		write(1, "token :\n", 8);
		while(cmd->token)
		{
			write(1, cmd->token->data, ft_strlen(cmd->token->data));
			write(1, "\n", 1);
			cmd->token = cmd->token->next;
		}
		cmd = cmd->next;
	}
}

void	print_token(t_token *token)
{
	write(1, "token :\n", 8);
	while (token)
	{
		if (token->data)
			write(1, token->data, ft_strlen(token->data));
		write(1, "\n", 1);
		token = token->next;
	}
}

void	cmd_token(t_cmd **cmd)
{
	t_cmd *tmp;

	tmp = *cmd;
	while(tmp)
	{
		tmp->token = ft_lst_create_token(0, 0);
		lexer_build(tmp->cmd, &tmp->token);
		tmp = tmp->next;
	}
}

int	check_token(t_token *token)
{
	int operator;

	if (token->type != 1)
	{			
		write(2, "Error syntax ", 13);
		write(2, token->data, ft_strlen(token->data));
		write(2, "\n", 1);
		return (1);
	}
	operator = 0;
	while (token)
	{
		if (token->type != 1)
			operator++;
		else
			operator = 0;
		if (operator > 1)
		{
			write(2, "Error syntax ", 13);
			write(2, token->data, ft_strlen(token->data));
			write(2, "\n", 1);
			return (1);		 
		}
		token = token->next;
	}
	return (0);
}

int	check_token_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		if (check_token(cmd->token))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char *rdl;
	t_cmd *cmd;

	(void)argc;
	(void)argv;
	(void)env;
	while (1)
	{
		rdl = readline("$_minishell_>>");
		add_history(rdl);
		while (is_char_in_set(*rdl, " \t"))
			rdl++;
		if (rdl[0] && !check_syntax_pipe(rdl) && !is_open_quote(rdl))
		{
			cmd = (t_cmd *)malloc(sizeof(t_cmd));
			if (!cmd)
				return (1);
			ft_memset(cmd, 0, sizeof(cmd));
			if(split_cmd(rdl, &cmd))
			{
				ft_free_cmd(&cmd);
				return (1);
			}
			cmd_token(&cmd);
			if (!check_token_cmd(cmd))
				print_cmd_token(cmd);
			ft_free_cmd(&cmd);
		}			
	}
	return (0);
}