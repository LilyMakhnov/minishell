/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:48:04 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 10:48:35 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_err(char *str, int i)
{
	i++;
	while (str[i] && is_char_in_set(str[i], " \t"))
		i++;
	if (str[i] && str[i] == '|')
		return (1);
	return (0);
}

int	check_syntax_pipe(char *rdl) // too many lines
{
	char	*str;
	int		ret;
	int		i;

	i = 0;
	ret = 0;
	str = ft_strtrim(rdl, " \t\n");
	if (str[i] && is_char_in_set(str[i], "|"))
		ret = str[i];
	while (str[i])
	{
		if (check_pipe_err(str + i, get_index_pipe(str + i)))
			ret = '|';
		i++;
	}
	if (is_char_in_set(str[i - 1], "|"))
		ret = str[i - 1];
	free(str);
	if (ret > 0)
	{
		write(2, "syntax error near unexpected token ",
			ft_strlen("syntax error near unexpected token "));
		write(2, &ret, 1);
		write(2, "\n", 1);
		g_exit_status = 2;
	}
	return (ret);
}
