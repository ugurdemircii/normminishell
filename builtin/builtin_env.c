#include "../minishell.h"

static void print_env(t_env *env) 
{
	while (env) 
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int builtin_env(t_env *env,char **args) 
{
	if (args[1])
	{
		ft_printf("'%s': No such file or directory\n",args[1]);
		return (127);
	}
	print_env(env);
	return 0;
}