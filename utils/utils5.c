#include "../minishell.h"

static void	free_all(char **result, int i)
{
	while (i-- > 0)
		free(result[i]);
	free(result);
}

static char	**words(char **result, char *s, char c)
{
	int		i;
	char	*start;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s == '\0')
			break ;
		start = s;
		while (*start && *start != c)
			start++;
		result[i] = ft_substr(s, 0, start - s);
		if (!result[i])
		{
			free_all(result, i);
			return (NULL);
		}
		i++;
		s = start;
	}
	result[i] = NULL;
	return (result);
}

static int	word_count(char *s, char c)
{
	int	word;
	int	count;

	word = 0;
	count = 0;
	while (*s)
	{
		if (*s != c && !count)
		{
			count = 1;
			word++;
		}
		else if (*s == c && *s)
		{
			count = 0;
		}
		s++;
	}
	return (word);
}

char	**ft_split(char *s, char c)
{
	char	**str;
	int		len;
	char	*tmp;

	tmp = (char *)s;
	len = word_count(s, c);
	str = ft_calloc(len + 1, sizeof(char *));
	if (!str)
		return (NULL);
	str = words(str, tmp, c);
	return (str);
}