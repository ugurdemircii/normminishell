/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:19:00 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 01:12:31 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	find_pipe(char *input, int *i)
{
	while (input[(*i)])
	{
		if (input[*i] == 34 || input[*i] == 39)
			go_next_quote(input, i);
		if (input[*i] == '|')
			break ;
		(*i)++;
	}
}

char	*extract_arg(char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]))
	{
		if (input[*i] == '"' || input[*i] == '\'')
			go_next_quote(input, i);
		else
			(*i)++;
	}
	return (ft_substr(input, start, *i - start));
}

static t_cmds	*minishell_split(char *input)
{
	t_cmds	*node;
	int		i;
	int		j;

	j = 0;
	i = 0;
	node = ft_calloc(1, sizeof(t_cmds));
	if (!node)
		return (NULL);
	node->command = ft_calloc(count_arg(input) + 1, sizeof(char *));
	if (!node->command)
		return (NULL);
	while (input[i])
	{
		while (ft_isspace(input[i]) && input[i])
			i++;
		if (input[i])
			node->command[j++] = extract_arg(input, &i);
	}
	node->command[j] = NULL;
	node->next = NULL;
	return (node);
}

static char	**pipe_split(char *input)
{
	int		i;
	char	**pipe_args;
	int		j;
	int		start;

	pipe_args = ft_calloc(pipe_count(input) + 2, sizeof(char *));
	if (!pipe_args)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		start = i;
		find_pipe(input, &i);
		pipe_args[j] = ft_substr(input, start, i - start);
		j++;
		if (input[i] == '\0')
			break ;
		i++;
	}
	pipe_args[j] = NULL;
	return (pipe_args);
}

void	create_t_cmds_node(t_cmds **cmd, char *input)
{
	int		i;
	char	*adjust_redirects;
	char	**divided_by_pipe;
	t_cmds	*node;

	adjust_redirects = re_write_redirect(input);
	divided_by_pipe = pipe_split(adjust_redirects);
	free(adjust_redirects);
	i = 0;
	while (divided_by_pipe[i])
	{
		node = minishell_split(divided_by_pipe[i]);
		add_cmd_node(cmd, node);
		i++;
	}
	free_double_ptr(divided_by_pipe);
	return ;
}
