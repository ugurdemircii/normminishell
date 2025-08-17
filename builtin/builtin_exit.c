#include "../minishell.h"

static int is_numeric(const char *str)
{
	int i = 0;

	if (!str)
		return 0;

	if (str[i] == '+' || str[i] == '-')
		i++;

	while (str[i])
	{
		if (!isdigit((unsigned char)str[i]))
			return 0;
		i++;
	}
	return 1;
}

static long long	ft_atoll(const char *str)
{
	long long	result;
	int			sign;

    result = 0;
    sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && *str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

int builtin_exit(char **args,t_cmds *cmd,t_env *env,int flag)
{
	int exit;

	if (flag == 1)
		write (1,"exit\n",5);
	if (!args[1])
		free_and_exit(cmd,env,0);
	if (args[1] && args[1][0] == '\0')
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
		free_and_exit(cmd,env,2);
	}
	if (!is_numeric(args[1]))
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
		free_and_exit(cmd,env,2);
	}
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		free_and_exit(cmd,env,1);
	}
	exit = (unsigned char)ft_atoll(args[1]); 
	free_and_exit(cmd,env,exit);
	return (0);
}