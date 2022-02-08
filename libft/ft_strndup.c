#include "libft.h"

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
