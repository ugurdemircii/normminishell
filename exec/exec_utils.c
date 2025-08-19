#include "../minishell.h"

char *find_executable(char *cmd, t_env *env)
{
	char *path;
	char **dirs;
	char *full_path;
	int i;

	path = find_in_env_list(env, "PATH");
	if (!path || ft_strchr(cmd, '/'))
		return ft_strdup(cmd);
	dirs = ft_split(path, ':');
	i = -1;
	while (dirs[++i])
	{
		full_path = ft_strjoin(dirs[i], cmd, '/');
		if (!full_path)
			return NULL;
		if (access(full_path, X_OK) == 0)
		{
			free_double_ptr(dirs);
			return full_path;
		}
		free(full_path);
	}
	free_double_ptr(dirs);
	return NULL;
}

void setup_pipes(t_cmds *cmd, int prev_fd, int pipefd[2],t_env *env)
{
	if (cmd->next)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		{
			perror("dup2 stdout");
			free_and_exit(cmd,env,1);
		}
	}
	if (prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) < 0)
		{
			perror("dup2 stdin");
			free_and_exit(cmd,env,1);
		}
	}
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
	if (prev_fd != -1)
		close(prev_fd);
}

int create_pipe(t_cmds *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) < 0)
		{
			perror("pipe");
			return (-1);
		}
	}
	else
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
	}
	return (0);
}

static void join_key_value(t_env *env, char **envp, int *count)
{
    char *line;

    if (env->value)
        line = ft_strjoin(env->key, env->value, '=');
    else
        line = ft_strdup(env->key);
    if (!line)
        return;
    envp[(*count)++] = line;
}

char **env_to_envp(t_env *env) 
{
	int count;
	char **envp;

	count = env_len(env);
	envp = ft_calloc(count + 1, sizeof(char *));
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