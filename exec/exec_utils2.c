#include "../minishell.h"

void close_unused_fds(int *prev_fd, t_cmds *cmd, int pipefd[2])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->next)
	{
		*prev_fd = pipefd[0];
		close(pipefd[1]);
	}
}

static int is_directory(const char *path) 
{
	struct stat st;
	if (stat(path, &st) == -1)
		return 0;
	return S_ISDIR(st.st_mode);
}

void handle_command_access(t_cmds *cmd,t_env *env)
{
	if (is_directory(cmd->command[0]))
	{
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->command[0], 2);
		ft_putendl_fd(": Is a directory", 2);
		free_and_exit(cmd,env,126);
	}
	if (access(cmd->command[0], F_OK) != 0)
	{
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->command[0], 2);
		ft_putendl_fd(": No such file or directory", 2);
		free_and_exit(cmd,env,127);
	}
	if (access(cmd->command[0], X_OK) != 0)
	{
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->command[0], 2);
		ft_putendl_fd(": Permission denied", 2);
		free_and_exit(cmd,env,126);
	}
}
