#include "../minishell.h"

static void join_key_value(t_env *env,char **envp,int *count)
{
	char *line;

	line = malloc(ft_strlen(env->key) + ft_strlen(env->value) + 2);
	if (!line)
		return ;
	sprintf(line, "%s=%s", env->key, env->value);
	envp[(*count)++] = line;
}

char **env_to_envp(t_env *env) 
{
	int count;
	t_env *tmp;
	char **envp;
	char *line;

	tmp = env;
	count = 0;
	while (tmp) 
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return NULL;
	count = 0;
	while (env) 
	{
		join_key_value(env, envp, &count);
		env = env->next;
	}
	envp[count] = NULL;
	return envp;
}