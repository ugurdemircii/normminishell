#include "../minishell.h"


void free_double_ptr(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		if (args[i])
			free(args[i]);
		i++;
	}
	free(args);	
}

void free_env_list(t_env *env)
{
	t_env *tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void free_redirect_list(t_redirect *redirect)
{
	t_redirect *tmp;

	while (redirect)
	{
		tmp = redirect;
		redirect = redirect->next;
		if (tmp->file)
			free(tmp->file);
		if (tmp->heredoc_fd > 0)
			close(tmp->heredoc_fd);
		free(tmp);
	}
}


void free_cmds_list(t_cmds *cmd)
{
	t_cmds *tmp;
	int i;
	while (cmd)
	{
		tmp = cmd;
		cmd = cmd->next;
		if (tmp->command)
		{
			for (i = 0; tmp->command[i]; i++)
				free(tmp->command[i]);
			free(tmp->command);
		}
		if (tmp->token_type)
			free(tmp->token_type);
		if (tmp->redirect_list)
			free_redirect_list(tmp->redirect_list);
		free(tmp);
	}
}