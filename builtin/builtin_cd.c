/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 00:31:03 by eakkoc            #+#    #+#             */
/*   Updated: 2025/08/20 02:08:31 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_cwd(char **args, char **cwd)
{
	if (args[1] && args[2])
	{
		ft_printf("minishell: cd: too many arguments\n");
		return (1);
	}
	*cwd = getcwd(NULL, 0);
	if (!*cwd)
	{
		perror("cd: getcwd");
		return (1);
	}
	return (0);
}

static char	*get_path(char *arg, t_env *env)
{
	char	*home;
	char	*oldpwd;

	if (!arg || ft_strcmp(arg, "--") == 0)
	{
		home = find_in_env_list(env, "HOME");
		if (!home)
			ft_printf("minishell: cd: HOME not set\n");
		return (home);
	}
	if (ft_strcmp(arg, "-") == 0)
	{
		oldpwd = find_in_env_list(env, "OLDPWD");
		if (!oldpwd)
			ft_printf("minishell: cd: OLDPWD not set\n");
		else
		{
			ft_putstr_fd(oldpwd, 1);
			ft_putstr_fd("\n", 1);
		}
		return (oldpwd);
	}
	return (arg);
}

static int	path_change_update(char *path, char *old_pwd, t_env **env)
{
	char	*cwd;

	if (chdir(path) != 0)
	{
		ft_printf("minishell: cd: ");
		perror(path);
		return (1);
	}
	set_env_var(env, "OLDPWD", old_pwd);
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		set_env_var(env, "PWD", cwd);
		free(cwd);
	}
	return (0);
}

int	builtin_cd(char **args, t_env *env)
{
	char	*cwd;
	char	*target_path;
	int		result;

	if (get_cwd(args, &cwd))
		return (1);
	target_path = get_path(args[1], env);
	if (!target_path)
	{
		free(cwd);
		return (1);
	}
	result = path_change_update(target_path, cwd, &env);
	free(cwd);
	return (result);
}
