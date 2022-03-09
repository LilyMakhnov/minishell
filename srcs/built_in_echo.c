/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 09:40:50 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 13:47:33 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_var(char *str, t_env *env)
{
	while (env)
	{
		if(!ft_strncmp(env->value, str, ft_strlen(str))
						&& env->value[ft_strlen(str)] == '=')
		{
			write(1, env->value + ft_strlen(str) + 1,
				 ft_strlen(env->value + ft_strlen(str) + 1));
			return (1);
		}
		env = env->next;
	}
	return (0);
}

int	parse_var(char *cmd, int *j, t_env *env)
{
	char	**var;
	int		l;
	int		t;

	t = 0;
	var = ft_split(cmd + *j, '$');
	l = -1;
	while (var[++l])
	{
		t = print_var(var[l], env);
		(*j) += ft_strlen(var[l]);
		if (l > 0)
			(*j)++;
	}
	l = -1;
	while (var[++l])
		free(var[l]);
	free(var);
	return (t);
}

void	built_in_echo(char **cmd, t_env *env)
{
	int	i;
	int	j;
	int	t;

	i = 0;
	if (!ft_strncmp(cmd[1], "-n", 2))
		i++;
	while (cmd[++i])
	{
		j = 0;
		t = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] != '$')
			{
				write(1, &cmd[i][j], 1);
				t++;
			}
			else
				t += parse_var(cmd[i], &j, env);
			j++;
		}
		if (cmd[i + 1] && t > 0)
			write(1, " ", 1);
	}
	if (ft_strncmp(cmd[1], "-n", 2))
		write (1, "\n", 1);
}
