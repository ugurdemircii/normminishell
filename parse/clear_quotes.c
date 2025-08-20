/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:17:33 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 01:07:05 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ignore_quote(char *str)
{
	char	*new_str;
	char	quote_type;
	int		j;
	int		i;

	new_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote_type = str[i++];
			while (str[i] && str[i] != quote_type)
				new_str[j++] = str[i++];
			if (str[i] == quote_type)
				i++;
		}
		else
			new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}

static void	clear_for_redirect_list(t_redirect *tmp)
{
	char	*cleaned;

	if (tmp->type == 4 && tmp->file && (ft_strchr(tmp->file, 34)
			|| ft_strchr(tmp->file, 39)))
		tmp->heredoc_expand_flag = 0;
	else
		tmp->heredoc_expand_flag = 1;
	if (ft_strchr(tmp->file, '"') || ft_strchr(tmp->file, '\''))
	{
		cleaned = ignore_quote(tmp->file);
		free(tmp->file);
		tmp->file = cleaned;
	}
}

static void	clear_for_command(t_cmds *cmd, int i)
{
	char	*cleaned;

	if (ft_strchr(cmd->command[i], '"') || ft_strchr(cmd->command[i], '\''))
	{
		cleaned = ignore_quote(cmd->command[i]);
		free(cmd->command[i]);
		cmd->command[i] = cleaned;
	}
}

void	clear_quotes_of_cmd(t_cmds *cmd)
{
	t_redirect	*tmp;
	int			i;

	while (cmd)
	{
		i = 0;
		tmp = cmd->redirect_list;
		while (cmd->command[i])
		{
			clear_for_command(cmd, i);
			i++;
		}
		while (tmp)
		{
			clear_for_redirect_list(tmp);
			tmp = tmp->next;
		}
		cmd = cmd->next;
	}
}
