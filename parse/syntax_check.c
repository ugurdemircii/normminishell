/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:19:13 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 01:40:52 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_invalid_syntax(t_cmds *cmd, char *input, int i)
{
	if (cmd->command[i + 1] == NULL)
	{
		if (last_pipe(input) || cmd->next != NULL)
		{
			ft_printf("minishell: syntax error near unexpected token `|'\n");
			return (1);
		}
		ft_printf("minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	if (cmd->token_type[i + 1] >= 1 && cmd->token_type[i + 1] <= 4)
	{
		ft_printf("minishell: syntax error near unexpected token `%s'\n",
			cmd->command[i + 1]);
		return (1);
	}
	return (0);
}

static int	pipe_control(char *input)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(input);
	if (input[0] == '|' || input[len - 1] == '|')
		return (0);
	return (1);
}

static int	consecutive_pipe(char *input)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] == 34 || input[i] == 39)
			go_next_quote(input, &i);
		else if (input[i] == '|')
		{
			i++;
			while (input[i] && ft_isspace(input[i]))
				i++;
			if (input[i] == '|')
				return (0);
		}
		else
			i++;
	}
	return (1);
}

static int	unclosed_quotes(const char *input)
{
	char	quote_type;
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote_type = input[i];
			i++;
			while (input[i] && input[i] != quote_type)
				i++;
			if (!input[i])
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_syntax(char *input)
{
	if (unclosed_quotes(input))
	{
		ft_printf("unclosed quote\n");
		return (0);
	}
	if (!consecutive_pipe(input) || !pipe_control(input))
	{
		ft_printf("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	return (1);
}
