/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:18:46 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 05:13:57 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	parse_steps(char *input, t_cmds **cmd, t_env *env, int exit_status)
{
	t_cmds	*head;

	create_t_cmds_node(cmd, input);
	head = *cmd;
	token_arg(*cmd);
	(*cmd)->redirect_list = NULL;
	if (redirect_token_check(*cmd, input))
	{
		free_cmds_list(*cmd);
		return (1);
	}
	while (*cmd)
	{
		redirect_and_command(*cmd);
		*cmd = (*cmd)->next;
	}
	*cmd = head;
	clean_command(*cmd);
	(*cmd)->exit_status = exit_status;
	expand_cmd(*cmd, env);
	clear_quotes_of_cmd(*cmd);
	return (0);
}

static void	init_env_list(t_env **env_list, char **envp)
{
	int		i;
	char	*equal_pos;
	char	*key;
	char	*value;

	i = 0;
	*env_list = NULL;
	while (envp[i])
	{
		equal_pos = ft_strchr(envp[i], '=');
		if (equal_pos)
		{
			key = ft_substr(envp[i], 0, equal_pos - envp[i]);
			value = ft_strdup(equal_pos + 1);
			add_env_node(env_list, create_env_node(key, value));
			free(key);
			free(value);
		}
		i++;
	}
}

static char	*set_input_and_history(char *input, int *exit_status)
{
	char	*trimmed;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(input);
	while (ft_isspace(input[i]))
		i++;
	if (i == len)
	{
		input[0] = '\0';
		return (input);
	}
	if (g_exit_status_shell == 130)
	{
		g_exit_status_shell = 0;
		*exit_status = 130;
	}
	trimmed = ft_strtrim(input, " \t\n\v\f\r");
	free(input);
	if (ft_strlen(trimmed) > 0)
		add_history(trimmed);
	return (trimmed);
}

static int	before_exec(char *input, t_cmds **cmd, t_env *env, int *exit_status)
{
	if (!check_syntax(input))
	{
		free(input);
		*exit_status = 2;
		return (1);
	}
	if (parse_steps(input, cmd, env, *exit_status))
	{
		free(input);
		*exit_status = 2;
		return (1);
	}
	free(input);
	if (setup_heredocs(*cmd, env) == -1)
	{
		free_cmds_list(*cmd);
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_env		*env;
	t_cmds		*cmd;
	static int	exit_status = 0;

	(void)argc;
	(void)argv;
	init_env_list(&env, envp);
	setup_signals();
	while (1)
	{
		cmd = NULL;
		input = readline("\033[1;32mminishell\033[0m~$");
		if (!input)
			handle_exit(cmd, env, exit_status);
		input = set_input_and_history(input, &exit_status);
		if (input[0] == '\0')
			continue ;
		if (before_exec(input, &cmd, env, &exit_status))
			continue ;
		execute_command(cmd, env);
		exit_status = cmd->exit_status;
		free_cmds_list(cmd);
	}
	return (0);
}
