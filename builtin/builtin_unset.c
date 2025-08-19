#include "../minishell.h"

static int	unset_env_var(t_env **env, const char *key)
{
	t_env *curr;
	t_env *prev;

	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return (0);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}


int	builtin_unset(t_env **env, char **args)
{
	int	i;

	if (!args || !args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_varname(args[i]))
			return (127);
		else
			unset_env_var(env, args[i]);
		i++;
	}
	return (0);
}