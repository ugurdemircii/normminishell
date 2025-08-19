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

// static long long	ft_atoll(const char *str)
// {
// 	long long	result;
// 	int			sign;

//     result = 0;
//     sign = 1;
// 	while (*str == ' ' || (*str >= 9 && *str <= 13))
// 		str++;
// 	if (*str == '-' || *str == '+')
// 	{
// 		if (*str == '-')
// 			sign = -1;
// 		str++;
// 	}
// 	while (*str && *str >= '0' && *str <= '9')
// 	{
// 		result = result * 10 + (*str - '0');
// 		str++;
// 	}
// 	return (result * sign);
// }

// int builtin_exit(char **args,t_cmds *cmd,t_env *env,int flag)
// {
// 	int exit;

// 	if (flag == 1)
// 		write (1,"exit\n",5);
// 	if (!args[1])
// 		free_and_exit(cmd,env,0);
// 	if ((args[1] && args[1][0] == '\0') || !is_numeric(args[1]))
// 	{
//         ft_putstr_fd("minishell: exit: ",2);
//         ft_putstr_fd(args[1],2);
//         ft_putendl_fd(": numeric argument required",2);
// 		free_and_exit(cmd,env,2);
// 	}
// 	if (args[2])
// 	{
// 		ft_putendl_fd("minishell: exit: too many arguments",2);
// 		free_and_exit(cmd,env,1);
// 	}
// 	exit = (unsigned char)ft_atoll(args[1]); 
// 	free_and_exit(cmd,env,exit);
// 	return (0);
// }

static int	check_overflow(long long result, int digit, int sign)
{
	if (sign == 1 && result > (LLONG_MAX - digit) / 10)
		return (1);
	if (sign == -1 && result > (-(LLONG_MIN + digit) / 10))
		return (1);
	return (0);
}
static int	parse_sign(const char **str)
{
	int	sign;

	sign = 1;
	while (**str == ' ' || (**str >= 9 && **str <= 13))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			sign = -1;
		(*str)++;
	}
	return (sign);
}


static int	ft_atoll_hard(const char *str, long long *out)
{
	long long	result;
	int			sign;
	int			digit;

	result = 0;
	*out = 0;
	sign = parse_sign(&str);
	if (!*str)
		return (0);
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		digit = *str - '0';
		if (check_overflow(result, digit, sign))
			return (0);
		result = result * 10 + digit;
		str++;
	}
	*out = result * sign;
	return (1);
}


int	builtin_exit(char **args, t_cmds *cmd, t_env *env, int flag)
{
	long long	exit;

	if (flag == 1)
		write(1, "exit\n", 5);
	if (!args[1])
		free_and_exit(cmd, env, 0);
	if (args[1][0] == '\0' || !is_numeric(args[1])
		|| !ft_atoll_hard(args[1], &exit))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		free_and_exit(cmd, env, 2);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		free_and_exit(cmd, env, 1);
	}
	free_and_exit(cmd, env, (unsigned char)exit);
	return (0);
}
