#include <minishell.h>

char	*ft_strtrijoin(char *str1, char *str2, char *str3)
{
	char *bin;

	bin = malloc(ft_strlen(str1)
        + ft_strlen(str2)
            + ft_strlen(str3) + 1);
	if (!bin)
		return (NULL);
	ft_strlcpy(bin, str1, ft_strlen(str1) + 1);
	ft_strlcat(bin, str2, ft_strlen(bin) + ft_strlen(str2) + 1);
	ft_strlcat(bin, str3, ft_strlen(bin) + ft_strlen(str3) + 1);
	return (bin);
}