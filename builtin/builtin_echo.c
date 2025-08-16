#include "../minishell.h"

int	n_option(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **args, int exit_after)
{
	int	i;
	int	newline;

	i = 1;
	newline = 0;
	while (args[i] && n_option(args[i]))
	{
		newline = 1;
		i++;
	}

	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}

	if (!newline)
		printf("\n");
	return (0);
}