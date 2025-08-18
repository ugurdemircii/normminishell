#include "../minishell.h"

void wait_for_last_child(pid_t last_pid, t_cmds *cmd)
{
	int status;
	pid_t waited;

	while ((waited = wait(&status)) > 0)
	{
		if (waited == last_pid)
		{
			if (WIFSIGNALED(status))
			{
				int sig = WTERMSIG(status);
				if (sig == SIGQUIT)
					write(2, "Quit (core dumped)\n", 20);
				if (sig == SIGINT)
					write(2, "\n", 1);
				cmd->exit_status = 128 + sig;
			}
			else if (WIFEXITED(status))
				cmd->exit_status = WEXITSTATUS(status);
		}
	}
	setup_signals();
}

void run_path(t_cmds *cmd,char **envp,t_env *env)
{
	execve(cmd->command[0], cmd->command, envp);
	perror("execve");
	free_and_exit(cmd,env,127);
}