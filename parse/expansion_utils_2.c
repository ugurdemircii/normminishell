/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:18:10 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 01:08:41 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	put_var_value(t_expand *expand_info, char *var_value, char *arg)
{
	char	*new_result;
	int		len;

	len = ft_strlen(expand_info->result) + ft_strlen(var_value)
		+ ft_strlen(arg);
	new_result = ft_calloc(len + 1, sizeof(char));
	if (!new_result)
		return ;
	ft_memcpy(new_result, expand_info->result, ft_strlen(expand_info->result));
	ft_strcpy(&new_result[expand_info->j], var_value);
	free(expand_info->result);
	expand_info->result = new_result;
	expand_info->j += ft_strlen(var_value);
}

void	get_var_name(t_expand *expand_info, char *arg, char **var_name)
{
	int	k;
	int	start_index;

	start_index = expand_info->i;
	k = 0;
	while (is_valid_var_char(arg[expand_info->i++]))
		k++;
	expand_info->i = start_index;
	*var_name = ft_calloc(k + 1, sizeof(char));
	if (!*var_name)
		return ;
	k = 0;
	while (is_valid_var_char(arg[expand_info->i]))
		(*var_name)[k++] = arg[expand_info->i++];
	(*var_name)[k] = '\0';
	return ;
}
