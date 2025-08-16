#include "../minishell.h"

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
		free_envp(envp);
		return;
	}
	fork_and_run_loop(cmd, env, envp);
	restore_stdio(stdin_backup, stdout_backup);
	free_envp(envp);
}