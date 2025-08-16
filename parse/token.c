#include "../minishell.h"

static int	check_redirect_syntax(t_cmds *cmd, char *input)
{
	int i;

	i = -1;
	while (cmd->command[++i])
	{
		if (cmd->token_type[i] >= 1 && cmd->token_type[i] <= 4)
		{
			if (cmd->command[i + 1] == NULL && (is_last_index_pipe(input) || cmd->next != NULL))
			{
				printf("syntax error near unexpected token `|'\n");
				return (1);
			}
			if (cmd->command[i + 1] == NULL)
			{
				printf("syntax error near unexpected token `newline'\n");
				return (1);
			}
			if (cmd->token_type[i + 1] >= 1 && cmd->token_type[i + 1] <= 4)
			{
				printf("syntax error near unexpected token `%s'\n", cmd->command[i + 1]);
				return (1);
			}
		}
	}
	return (0);
}
void redirect_and_command(t_cmds *cmd)
{
    int i;
	t_redirect *new;

	i = 0;
    cmd->redirect_list = NULL;
    while (cmd->command[i])
    {
        if (cmd->token_type[i] >= 1 && cmd->token_type[i] <= 4)
        {
            new = create_new_redirect(cmd->token_type[i], cmd->command[i+1]);
            if (!new)
                return;
            add_redirect_node(&cmd->redirect_list, new);
            i += 2;
            continue;
        }
        i++;
    }
}

void token_arg(t_cmds *cmd)
{
	int i;

	while (cmd)
	{
		i = -1;
		cmd->token_type = malloc(sizeof(int) * count_len_double_ptr(cmd->command));//
		if (!cmd->token_type)
			return ;
		while (cmd->command[++i])
		{
			if (ft_strcmp(cmd->command[i], ">") == 0)
				cmd->token_type[i] = 1;
			else if (ft_strcmp(cmd->command[i], ">>") == 0)
				cmd->token_type[i] = 2;
			else if (ft_strcmp(cmd->command[i], "<") == 0)
				cmd->token_type[i] = 3;
			else if (ft_strcmp(cmd->command[i], "<<") == 0)
				cmd->token_type[i] = 4;
			else
				cmd->token_type[i] = 5;
		}
		cmd = cmd->next;
	}
}
int	redirect_token_check(t_cmds *cmd, char *input)
{
	while (cmd)
	{
		if (check_redirect_syntax(cmd, input))
			return (1);
		cmd = cmd->next;
	}
	return (0);
}
void clean_command(t_cmds *cmd)
{
	int i; 
	int j;
	char **new_cmd;

	while (cmd)
	{
		new_cmd = malloc(sizeof(char *) * (count_len_double_ptr(cmd->command) + 1));
		if (!new_cmd)
			return ;
		i = 0;
		j = 0;
		while (cmd->command[i])
		{
			if (cmd->token_type[i] >= 1 && cmd->token_type[i] <= 4)
			{
				i += 2;
				continue;
			}
			new_cmd[j++] = ft_strdup(cmd->command[i++]);
		}
		new_cmd[j] = NULL;
		free_double_ptr(cmd->command);
		cmd->command = new_cmd;
		cmd = cmd->next;
	}
}