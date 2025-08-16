#include "../minishell.h"

int is_builtin(char *cmd) 
{
	if (!cmd)
		return 0;
	return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd")
		|| !strcmp(cmd, "export") || !strcmp(cmd, "unset")
		|| !strcmp(cmd, "env") || !strcmp(cmd, "exit"));
}

int handle_single_builtin(t_cmds *cmd, t_env **env, char **envp)
{
	int builtin_return;

	if (setup_redirects(cmd->redirect_list) < 0)
		return (1);
	builtin_return = run_builtin(cmd,env,1);
	return (builtin_return);
}

int run_builtin(t_cmds *cmd, t_env **env,int flag) 
{
	if (!ft_strcmp(cmd->command[0], "echo"))
		return builtin_echo(cmd->command,cmd->exit_status);
	else if (!ft_strcmp(cmd->command[0], "cd"))
		return builtin_cd(cmd->command, *env);
	else if (!ft_strcmp(cmd->command[0], "pwd"))
		return builtin_pwd();
	else if (!ft_strcmp(cmd->command[0], "export"))
		return builtin_export(env,cmd->command);
	else if (!ft_strcmp(cmd->command[0], "unset"))
		return builtin_unset(env,cmd->command);
	else if (!ft_strcmp(cmd->command[0], "env"))
		return builtin_env(*env,cmd->command);
	else if (!ft_strcmp(cmd->command[0], "exit"))
		return builtin_exit(cmd->command,cmd,*env,flag);
	return (1);
}