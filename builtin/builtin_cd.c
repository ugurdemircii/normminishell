#include "../minishell.h"
#include <errno.h>

static int get_cwd(char **args, char *cwd)
{
    if (args[1] && args[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }
    if (!getcwd(cwd, PATH_MAX))
    {
        perror("cd: getcwd");
        return 1;
    }
    return 0;
}

static char *get_path(char *arg, t_env *env)
{
    if (!arg || ft_strcmp(arg, "--") == 0)
    {
        char *home = get_env_value(env, "HOME");
        if (!home)
            fprintf(stderr, "cd: HOME not set\n");
        return home;
    }
    if (ft_strcmp(arg, "-") == 0)
    {
        char *oldpwd = get_env_value(env, "OLDPWD");
        if (!oldpwd)
            fprintf(stderr, "cd: OLDPWD not set\n");
        else
            printf("%s\n", oldpwd);
        return oldpwd;
    }
    return arg;
}

static int path_change_update(char *path, char *old_pwd, t_env **env)
{
    if (chdir(path) != 0)
    {
        fprintf(stderr, "cd: %s: %s\n", path, strerror(errno));
        return 1;
    }  
    set_env_var(env, "OLDPWD", old_pwd);
    char cwd[PATH_MAX];
    if (getcwd(cwd, PATH_MAX))
        set_env_var(env, "PWD", cwd);
        
    return 0;
}

int builtin_cd(char **args, t_env *env)
{
    char cwd[PATH_MAX];
    char *target_path;

    if (get_cwd(args, cwd))
        return 1;
        
    target_path = get_path(args[1], env);
    if (!target_path)
        return 1;
        
    return path_change_update(target_path, cwd, &env);
}