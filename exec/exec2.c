/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakkoc <eakkoc@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:31:58 by eakkoc            #+#    #+#             */
/*   Updated: 2025/08/20 00:31:59 by eakkoc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_for_last_child(pid_t last_pid, t_cmds *cmd)
{
	int		status;
	pid_t	waited;
	int		sig;

	waited = wait(&status);
	while (waited > 0)
	{
		if (waited == last_pid)
		{
			if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				if (sig == SIGQUIT)
					write(2, "Quit (core dumped)\n", 20);
				if (sig == SIGINT)
					write(2, "\n", 1);
				cmd->exit_status = 128 + sig;
			}
			else if (WIFEXITED(status))
				cmd->exit_status = WEXITSTATUS(status);
		}
		waited = wait(&status);
	}
	setup_signals();
}

void	run_path(t_cmds *cmd, char **envp, t_env *env)
{
	execve(cmd->command[0], cmd->command, envp);
	perror("execve");
	free_and_exit(cmd, env, 127);
}
