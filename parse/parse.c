#include "../minishell.h"

char *extract_arg(char *input, int *i)
{
    int start;
    
    start = *i;
    while (input[*i] && !isspace(input[*i]))
    {
        if (input[*i] == '"' || input[*i] == '\'')
            go_next_quote(input, i);
        else
            (*i)++;
    }
    return ft_substr(input, start, *i - start);
}

static t_cmds *minishell_split(char *input)
{
    t_cmds *node;
    int i;
    int j;

	j = 0;
	i = 0;
    node = ft_calloc(1, sizeof(t_cmds));
    if (!node)
        return NULL;
    node->command = ft_calloc(count_arg(input) + 1, sizeof(char *));
    if (!node->command)
        return NULL;
    while (input[i])
    {
        while(isspace(input[i]) && input[i])
            i++;
		if (input[i])
			 node->command[j++] = extract_arg(input,&i);
    }
    node->command[j] = NULL;
    node->next = NULL;
    return (node);
}


static char    **pipe_split(char *input)
{
    int i;
    char **pipe_args;
    int j;
    int start;

    pipe_args = ft_calloc(pipe_count(input) + 2, sizeof(char *));
    if (!pipe_args)
        return NULL;
    i = 0;
    j = 0;
    while (input[i])
    {
        start = i;
        find_pipe(input,&i);
        pipe_args[j] = ft_substr(input, start, i - start);
        j++;
        if (input[i] == '\0')
            break ;
        i++;
    }
    pipe_args[j] = NULL;
    return (pipe_args);
}

static void	create_t_cmds_node(t_cmds **cmd,t_env *env,char *input)
{
	int i;
	char *adjust_redirects;
	char **divided_by_pipe;
	t_cmds *node;

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


int parse_steps(char *input,t_cmds **cmd, t_env *env,int exit_status)
{
	t_cmds *head;

	create_t_cmds_node(cmd,env,input);
	head = *cmd;
	token_arg(*cmd);
	(*cmd)->redirect_list = NULL;
	if (redirect_token_check(*cmd,input))
	{
		free_cmds_list(*cmd);
		return (1);
	}
	while (*cmd)
	{
		redirect_and_command(*cmd);
		*cmd = (*cmd)->next;
	}
	*cmd = head;
	clean_command(*cmd);
	(*cmd)->exit_status = exit_status;
	expand_cmd(*cmd, env);
	clear_quotes_of_cmd(*cmd);
	return (0);
}