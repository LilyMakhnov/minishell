#include "minishell.h"
/*
int is_in_quotes(int j, char *str)
{
	int i
	while (str[])
	str[i]
}*/

int	get_index_pipe(char *rdl)
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
		if (rdl[i] == '|' && quote == 0 && dquote == 0)
			return (i);	
		i++;
	}
	return (-1);
} 

int	add_next_cmd(char *rdl, t_cmd **cmd)
{
	int ip;
	char *tmp;

	ip = get_index_pipe(rdl);
	(*cmd)->next = 0;
	if (ip > 0)
		tmp = ft_strndup(rdl, ip + 1);
	else
		tmp = ft_strdup(rdl);
	(*cmd)->str = ft_strtrim(tmp, " \t");
	free(tmp);
	return (ip);
}

int	split_cmd(char *rdl, t_cmd **cmd)
{
	t_cmd *tmp;
	int is_pipe;

	tmp = (*cmd);
	is_pipe = add_next_cmd(rdl, &tmp);
	while (is_pipe > 0)
	{
		tmp->next = (t_cmd *)malloc(sizeof(t_cmd)); 
		if (!tmp->next)
			return (1);
		tmp = tmp->next;
		ft_memset(tmp, 0, sizeof(tmp));
		rdl = rdl + is_pipe + 1;
		is_pipe = add_next_cmd(rdl, &tmp);
	}
	return (0);
}

void	print_cmd(t_cmd *cmd)
{
	while (cmd)
	{
		write(1, "cmd:\n", 5);
		if (cmd->str)
			write(1, cmd->str, ft_strlen(cmd->str));
		write(1, "\n", 1);
		cmd = cmd->next;
	}
}