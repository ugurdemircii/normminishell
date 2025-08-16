#include "minishell.h"

static size_t	count_len(int n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (1);
	if (n == -2147483648)
	{
		n = 147483648;
		i = 2;
	}
	if (n < 0)
	{
		n *= -1;
		i = 1;
	}
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}
int is_last_index_pipe(char *input)
{
	int len;

	len = ft_strlen(input);
	if (input[len - 1] == '|')
		return (1);
	return (0);
}

static void	ft_bzero(void *s, size_t n)
{
	char	*str;
	size_t	i;

	i = 0;
	str = s;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		digit;

	digit = count_len(n);
	str = (char *)ft_calloc(digit + 1, sizeof(char));
	if (!str)
		return (NULL);
	if (n == 0)
		*str = '0';
	else if (n < 0)
	{
		if (n == -2147483648)
		{
			ft_strlcpy(str, "-2147483648", digit + 1);
			return (str);
		}
		str[0] = '-';
		n *= -1;
	}
	while (n != 0)
	{
		*(str + --digit) = (n % 10) + '0';
		n = n / 10;
	}
	return (str);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
