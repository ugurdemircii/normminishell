/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:31:47 by eakkoc            #+#    #+#             */
/*   Updated: 2025/08/20 04:19:53 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd,
			"env") || !ft_strcmp(cmd, "exit"));
}

int	run_builtin(t_cmds *cmd, t_env **env, int flag)
{
	if (!ft_strcmp(cmd->command[0], "echo"))
		return (builtin_echo(cmd->command));
	else if (!ft_strcmp(cmd->command[0], "cd"))
		return (builtin_cd(cmd->command, *env));
	else if (!ft_strcmp(cmd->command[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->command[0], "export"))
		return (builtin_export(env, cmd->command));
	else if (!ft_strcmp(cmd->command[0], "unset"))
		return (builtin_unset(env, cmd->command));
	else if (!ft_strcmp(cmd->command[0], "env"))
		return (builtin_env(*env, cmd->command));
	else if (!ft_strcmp(cmd->command[0], "exit"))
		return (builtin_exit(cmd->command, cmd, *env, flag));
	return (1);
}

int	handle_single_builtin(t_cmds *cmd, t_env **env)
{
	int	builtin_return ;

	if (setup_redirects(cmd->redirect_list) < 0)
		return (1);
	builtin_return = run_builtin(cmd, env, 1);
	return (builtin_return);
}
