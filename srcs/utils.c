/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 10:23:19 by crondeau          #+#    #+#             */
/*   Updated: 2022/03/09 11:58:07 by crondeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*ft_strtrijoin(char *str1, char *str2, char *str3)
{
	char	*bin;

	bin = malloc(ft_strlen(str1) + ft_strlen(str2)
			+ ft_strlen(str3) + 1);
	if (!bin)
		return (NULL);
	ft_strlcpy(bin, str1, ft_strlen(str1) + 1);
	ft_strlcat(bin, str2, ft_strlen(bin) + ft_strlen(str2) + 1);
	ft_strlcat(bin, str3, ft_strlen(bin) + ft_strlen(str3) + 1);
	return (bin);
}

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
	while (src[i] && --n + 1)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
