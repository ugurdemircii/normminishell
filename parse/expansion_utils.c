#include "../minishell.h"

bool is_valid_var_char(char c) 
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') || c == '_';
}

char *find_in_env_list(t_env *env_list, char *var_name)
{
    while (env_list) 
	{
        if (ft_strcmp(env_list->key, var_name) == 0)
            return env_list->value;
        env_list = env_list->next;
    }
    return NULL;
}


void handle_quote(t_expand *expand_info, char c) 
{
    if (c == '\'' && !expand_info->in_double) 
        expand_info->in_single = !expand_info->in_single;
    else if (c == '"' && !expand_info->in_single) 
        expand_info->in_double = !expand_info->in_double;
    expand_info->result[expand_info->j++] = c;
}

void	handle_exit_code(t_expand *expand_info, int exit_status)
{
	char	*code_str;

	code_str = ft_itoa(exit_status);
	if (!code_str)
		return ;
	ft_strcpy(&expand_info->result[expand_info->j], code_str);
	expand_info->j += ft_strlen(code_str);
	free(code_str);
	expand_info->i++;
}
