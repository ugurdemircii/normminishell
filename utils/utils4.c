#include "../minishell.h"

int count_len_double_ptr(char **args)
{
	int i;
	
	i = 0;
	while (args[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((*s == (char)c))
		return ((char *)s);
	return (NULL);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}
void ft_putendl_fd(char *s,int fd)
{
	if (!s)
		return ;
	ft_putstr_fd(s,fd);
	write(fd,"\n",1);
}

char	*ft_strtrim(char *s1, char *set)
{
	char	*trimmed;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (NULL);
	end = ft_strlen(s1);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		++start;
	if (end > 0)
	{
		end--;
		while (s1[end] && ft_strchr(set, s1[end]))
			end--;
	}
	if (start > end)
		return (ft_substr(s1, start, 0));
	trimmed = ft_substr(s1, start, end - start + 1);
	return (trimmed);
}