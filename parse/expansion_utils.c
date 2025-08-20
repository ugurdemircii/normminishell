/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:18:23 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 06:11:22 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_valid_var_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9') || c == '_');
}

char	*find_in_env_list(t_env *env_list, char *var_name)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, var_name) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

void	handle_quote(t_expand *expand_info, char c)
{
	if (c == '\'' && !expand_info->in_double)
		expand_info->in_single = !expand_info->in_single;
	else if (c == '"' && !expand_info->in_single)
		expand_info->in_double = !expand_info->in_double;
	expand_info->result[expand_info->j++] = c;
}

void	handle_exit_code(t_expand *expand_info, int exit_status, char *arg)
{
	char	*code_str;
	char	*new_result;
	int		len;

	code_str = ft_itoa(exit_status);
	if (!code_str)
		return ;
	len = ft_strlen(expand_info->result) + ft_strlen(arg)
		+ ft_strlen(code_str);
	new_result = ft_calloc(len + 1, sizeof(char));
	if (!new_result)
		return ;
	ft_memcpy(new_result, expand_info->result, ft_strlen(expand_info->result));
	ft_strcpy(&new_result[expand_info->j], code_str);
	free(expand_info->result);
	expand_info->result = new_result;
	expand_info->j += ft_strlen(code_str);
	free(code_str);
	expand_info->i++;
}
