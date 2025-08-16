#include "../minishell.h"




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