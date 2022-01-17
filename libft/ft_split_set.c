#include "libft.h"

static int		is_in_char(char a, char *charset)
{
	while (*charset)
	{
		if (*charset == a)
			return (1);
		charset++;
	}
	return (0);
}

static int		ft_count_word(char *str, char *charset)
{
	int wordscount;
	int inword;
	int i;

	i = -1;
	wordscount = 0;
	inword = 0;
	while (str[++i])
	{
		if (is_in_char(str[i], charset))
			inword = 0;
		else if (inword == 0)
		{
			wordscount++;
			inword = 1;
		}
	}
	return (wordscount);
}

static int		size_w(int *i, char *str, char *charset)
{
	int j;

	j = 0;
	while (str[*i] && is_in_char(str[*i], charset))
		(*i)++;
	while (str[*i] && !is_in_char(str[*i], charset))
	{
		j++;
		(*i)++;
	}
	return (j);
}

static void	ft_copy(char *dest, int *i, char *str, char *charset)
{
	int j;

	j = 0;
	while (str[*i] && is_in_char(str[*i], charset))
		(*i)++;
	while (str[*i] && !is_in_char(str[*i], charset))
	{
		dest[j++] = str[*i];
		(*i)++;
	}
	dest[j] = 0;
}

char	**ft_split_set(char *str, char *charset)
{
	char	**dest;
	int		pos;
	int		i;
	int		size_word;

	if (!(dest = malloc(sizeof(char*) * (ft_count_word(str, charset) + 1))))
		return (NULL);
	i = 0;
	pos = 0;
	while (i < ft_count_word(str, charset))
	{
		size_word = size_w(&pos, str, charset);
		dest[i] = malloc(sizeof(char)
				* (size_word) + 1);
		if (!dest[i])
			return (NULL);
		i++;
	}
	i = 0;
	pos = 0;
	while (i < ft_count_word(str, charset))
		ft_copy(dest[i++], &pos, str, charset);
	dest[i] = 0;
	return (dest);
}
