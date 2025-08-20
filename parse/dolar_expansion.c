/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dolar_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:18:00 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 06:12:17 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	expand_var(t_expand *expand_info, char *arg, t_env *env_list,
		t_cmds *cmd)
{
	char	*var_value;
	char	*var_name;

	expand_info->i++;
	if (arg[expand_info->i] == '?')
	{
		handle_exit_code(expand_info, cmd->exit_status, arg);
		return ;
	}
	get_var_name(expand_info, arg, &var_name);
	if (!var_name)
		return ;
	var_value = find_in_env_list(env_list, var_name);
	if (var_value)
		put_var_value(expand_info, var_value, arg);
	free(var_name);
}

char	*expand_arg(char *arg, t_env *env_list, int num, t_cmds *cmd)
{
	t_expand	expand_info;

	expand_info.result = ft_calloc(ft_strlen(arg) + 1, sizeof(char));
	expand_info.i = 0;
	expand_info.j = 0;
	expand_info.in_single = false;
	expand_info.in_double = false;
	while (arg[expand_info.i])
	{
		if ((arg[expand_info.i] == '\'' || arg[expand_info.i] == '"')
			&& num == 1)
			handle_quote(&expand_info, arg[expand_info.i++]);
		else if (arg[expand_info.i] == '$' && !expand_info.in_single
			&& arg[expand_info.i + 1] != '$'
			&& (is_valid_var_char(arg[expand_info.i + 1]) || arg[expand_info.i
					+ 1] == '?'))
			expand_var(&expand_info, arg, env_list, cmd);
		else
			expand_info.result[expand_info.j++] = arg[expand_info.i++];
	}
	if (expand_info.result != NULL)
		expand_info.result[expand_info.j] = '\0';
	return (expand_info.result);
}

static char	**expand_command(t_cmds *cmd, t_env *env_list)
{
	char	**new_command;
	char	*expanded;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new_command = ft_calloc(count_double_ptr(cmd->command) + 1,
			sizeof(char *));
	if (!new_command)
		return (NULL);
	while (cmd->command[i])
	{
		expanded = expand_arg(cmd->command[i], env_list, 1, cmd);
		if (expanded[0] != '\0')
			new_command[j++] = expanded;
		else
			free(expanded);
		i++;
	}
	new_command[j] = NULL;
	return (new_command);
}

static void	expand_redirect_list(t_cmds *cmd, t_env *env_list)
{
	t_redirect	*tmp;
	char		*expanded;

	tmp = cmd->redirect_list;
	while (tmp)
	{
		if (tmp->type != 4)
		{
			expanded = tmp->file;
			tmp->file = expand_arg(tmp->file, env_list, 1, cmd);
			free(expanded);
		}
		tmp = tmp->next;
	}
}

void	expand_cmd(t_cmds *cmd, t_env *env_list)
{
	char	**new_command;

	while (cmd)
	{
		new_command = expand_command(cmd, env_list);
		if (new_command)
		{
			free_double_ptr(cmd->command);
			cmd->command = new_command;
		}
		expand_redirect_list(cmd, env_list);
		cmd = cmd->next;
	}
}
