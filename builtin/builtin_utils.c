#include "../minishell.h"

int	is_valid_varname(const char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!(isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!(isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

char	*extract_varvalue(char *arg)
{
	int		i;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (!arg[i])
		return (NULL);
	return (ft_strdup(&arg[i + 1]));
}

char	*extract_varname(char *arg)
{
	int		i;
	char	*name;

	if (!arg)
		return (NULL);
	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_calloc((i + 1),sizeof(char));
	if (!name)
		return (NULL);
	strncpy(name, arg, i);
	name[i] = '\0';
	return (name);
}

t_env	**env_to_array(t_env *env)
{
	t_env	**array;
	int		i;

	array = ft_calloc((env_len(env) + 1), sizeof(t_env *));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		array[i++] = env;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

void	sort_env_array(t_env **array)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (array[i])
	{
		j = i + 1;
		while (array[j])
		{
			if (ft_strcmp(array[i]->key, array[j]->key) > 0)
			{
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
			j++;
		}
		i++;
	}
}