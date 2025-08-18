#include "../minishell.h"

static void run_command(t_cmds *cmd, t_env *env)
{
	int builtin_return;
	char *full_path;

    if (cmd->command[0] == NULL)
        free_and_exit(cmd, env, 0);
    else if (is_builtin(cmd->command[0]))
    {
        builtin_return = run_builtin(cmd, &env,0);
        free_and_exit(cmd, env, builtin_return);
    }
	else if (ft_strchr(cmd->command[0], '/'))
    {
        handle_command_access(cmd, env);
        run_path(cmd, cmd->envp, env);
    }
	full_path = find_executable(cmd->command[0], env);
    if (!full_path)
    {
        ft_printf("%s: command not found\n", cmd->command[0]);
        free_and_exit(cmd, env, 127);
    }
    execve(full_path, cmd->command, cmd->envp);
    perror("execve");
    free(full_path);
    free_and_exit(cmd, env, 127);
}

static void child_process(t_cmds *cmd, int prev_fd, int pipefd[2], t_env *env)
{
    setup_pipes(cmd, prev_fd, pipefd, env);
    if (setup_redirects(cmd->redirect_list) < 0)
        free_and_exit(cmd, env, 1);
    
    run_command(cmd, env);
}

static pid_t fork_child_and_run(t_cmds *cmd, int prev_fd, int pipefd[2], t_env *env)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
	{
		setup_exec_signals();
		child_process(cmd, prev_fd, pipefd, env);
	}
	else
		ignore_signals();
	return (pid);
}

static void fork_and_run_loop(t_cmds *cmd, t_env *env, char **envp)
{
	t_cmds *curr;
	int pipefd[2];
	int prev_fd;
	pid_t pid;
	pid_t last_pid;

	curr = cmd;
	prev_fd = -1;
	last_pid = -1;
	while (curr)
	{
		if (create_pipe(curr, pipefd) < 0)
			break;
		curr->head = cmd;
		curr->envp = envp;
		pid = fork_child_and_run(curr, prev_fd, pipefd,env);
		if (pid < 0)
			break;
		last_pid = pid;
		close_unused_fds(&prev_fd, curr, pipefd);
		curr = curr->next;
	}
	wait_for_last_child(last_pid,cmd);
}

void execute_command(t_cmds *cmd, t_env *env)
{
	char **envp;
	int stdin_backup;
	int stdout_backup;

	envp = env_to_envp(env);
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (!cmd || !envp)
		return;
	if (!cmd->next && is_builtin(cmd->command[0]))
	{
		cmd->head = cmd;
		cmd->envp = envp;
		cmd->exit_status = handle_single_builtin(cmd, &env, envp);
		restore_stdio(stdin_backup, stdout_backup);
		free_double_ptr(envp);
		return;
	}
	fork_and_run_loop(cmd, env, envp);
	restore_stdio(stdin_backup, stdout_backup);
	free_double_ptr(envp);
}