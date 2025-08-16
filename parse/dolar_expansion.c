#include "../minishell.h"



static void expand_var(t_expand *expand_info, const char *arg, t_env *env_list,t_cmds *cmd) 
{
    expand_info->i++;
    int k;
	char *var_value;

    k = 0;
    if (arg[expand_info->i] == '?') 
	{
        handle_exit_code(expand_info,cmd->exit_status);
        return;
    }
    char var_name[256] = {0};
    while (is_valid_var_char(arg[expand_info->i]))
        var_name[k++] = arg[expand_info->i++];
    var_value = find_in_env_list(env_list, var_name);
    if (var_value) 
    {
        ft_strcpy(&expand_info->result[expand_info->j], var_value);
        expand_info->j += ft_strlen(var_value);
    }
}

char *expand_arg(const char *arg, t_env *env_list, int num,t_cmds *cmd) 
{
    int len;
    t_expand expand_info;

	len = ft_strlen(arg);
    expand_info.result = malloc(len * 100 + 1);
    expand_info.i = 0;
    expand_info.j = 0;
    expand_info.in_single = false;
    expand_info.in_double = false;
    while (arg[expand_info.i]) 
    {
        if ((arg[expand_info.i] == '\'' || arg[expand_info.i] == '"') && num == 1) 
            handle_quote(&expand_info, arg[expand_info.i++]);
        else if (arg[expand_info.i] == '$' && !expand_info.in_single 
            && arg[expand_info.i + 1] != '$' && (is_valid_var_char(arg[expand_info.i + 1]) || arg[expand_info.i + 1] == '?'))
            expand_var(&expand_info, arg, env_list,cmd);
        else
            expand_info.result[expand_info.j++] = arg[expand_info.i++];
    }
    if (expand_info.result != NULL)
        expand_info.result[expand_info.j] = '\0';
    return expand_info.result;
}


static char **expand_command(t_cmds *cmd, t_env *env_list)
{
    char **new_command;
    char *expanded;
    int i = 0;
    int j = 0;

    new_command = malloc(sizeof(char *) * 10000);
    if (!new_command)
        return NULL;
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
    return new_command;
}


static void expand_redirect_list(t_cmds *cmd, t_env *env_list)
{
    t_redirect *tmp = cmd->redirect_list;
    char *expanded;

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

void expand_cmd(t_cmds *cmd, t_env *env_list)
{
    char **new_command;

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