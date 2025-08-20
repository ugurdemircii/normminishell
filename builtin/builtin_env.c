/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakkoc <eakkoc@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:31:11 by eakkoc            #+#    #+#             */
/*   Updated: 2025/08/20 00:31:12 by eakkoc           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

int	builtin_env(t_env *env, char **args)
{
	if (args[1])
	{
		ft_printf("'%s': No such file or directory\n", args[1]);
		return (127);
	}
	print_env(env);
	return (0);
}
