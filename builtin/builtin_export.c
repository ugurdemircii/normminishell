#include "../minishell.h"

static int export_error(t_env **env, char **args,int *i)
{
	char *key;
	char *value;

	key = extract_varname(args[*i]);
	if (!key || !is_valid_varname(key))
	{
        ft_putstr_fd("export: `",2);
        ft_putendl_fd(args[*i], 2);
        ft_putendl_fd("': not a valid identifier", 2);
		free(key);
		return (1);
	}
	value = extract_varvalue(args[*i]);
	if (set_env_var(env, key, value) == -1)
	{
		ft_putendl_fd("export: memory allocation error", 2);
		free(key);
		free(value);
		return (1);
	}
	free(key);
	free(value);
	return (0);
}
static void	print_exported_vars(t_env *env)
{
	t_env	**array;
	int		i;

	array = env_to_array(env);
	if (!array)
		return ;
	sort_env_array(array);
	i = 0;
	while (array[i])
	{
		if (array[i]->value)
			printf("declare -x %s=\"%s\"\n",
				array[i]->key, array[i]->value);
		else
			printf("declare -x %s\n", array[i]->key);
		i++;
	}
	free(array);
}
static int export_only(char **args,t_env *env)
{
    if (!args || !args[1])
	{
		print_exported_vars(env);
		return (1);
	}
    return (0);
}

int	builtin_export(t_env **env, char **args)
{
	int		i;
	char	*key;
	char	*value;
	int exit;

	exit = 0;
	i = 1;
    if (export_only(args, *env) == 1)
        return (0);
	// if (!args || !args[1])
	// {
	// 	print_exported_vars(*env);
	// 	return (0);
	// }
	while (args[i])
	{
		if (!args[i][0])
		{
            ft_putstr_fd("export: `",2);
            ft_putstr_fd(args[i],2);
            ft_putendl_fd("': not a valid identifier", 2);
            i++;
			exit = 1;
			continue ;
		}
		if (export_error(env, args, &i) == 1)
			return (1);
		i++;
	}
	return (exit);
}