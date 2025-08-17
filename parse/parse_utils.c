#include "../minishell.h"



t_cmds *minishell_split(char *input)
{
    t_cmds *node;
    int i;
    int j;

    node = ft_calloc(1, sizeof(t_cmds));
    if (!node)
        return NULL;
    node->command = ft_calloc(ft_strlen(input) + 1, sizeof(char *));
    if (!node->command)
        return NULL;
    j = 0;
    i = 0;
    while (input[i])
    {
        while(isspace(input[i]) && input[i])
            i++;
        if ((input[i] != 34 && input [i] != 39) && input[i] != '\0')
            node->command[j++] = no_quote(input,&i);
        if ((input[i] == '"' || input [i] == '\'') && input[i] != '\0')
            node->command[j++] = quote(input,&i,input[i]);
    }
    node->command[j] = NULL;
    node->next = NULL;
    return (node);
}

char    **pipe_split(char *input)
{
    int i;
    char **pipe_args;
    int j;
    int start;

    pipe_args = ft_calloc(100000, sizeof(char *));
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

static int handle_quotes(char current_char, char *quote_flag, char *output, int *j)
{
    if ((current_char == '\'' || current_char == '"') && (*quote_flag == 0 || *quote_flag == current_char))
    {
        if (*quote_flag != 0)
            *quote_flag = 0;
        else
            *quote_flag = current_char;
        
        output[(*j)++] = current_char;
        return 1;
    }
    return 0;
}

static void handle_redirection(const char *input, int *i, char *output, int *j)
{
    if ((input[*i] == '>' && input[*i+1] == '>') || (input[*i] == '<' && input[*i+1] == '<'))
    {
        output[(*j)++] = ' ';
        output[(*j)++] = input[*i];
        output[(*j)++] = input[*i];
        output[(*j)++] = ' ';
        *i += 2;
    }
    else if (input[*i] == '>' || input[*i] == '<')
    {
        output[(*j)++] = ' ';
        output[(*j)++] = input[(*i)++];
        output[(*j)++] = ' ';
    }
}

char *re_write_redirect(char *input)
{
    int i;
    int j;
    char *output;
    char quote_flag;

    i = 0;
    j = 0;
    quote_flag = 0;
    output = ft_calloc(strlen(input) * 4 + 1, sizeof(char));
    if (output == NULL)
        return NULL;
    while (input[i])
    {
        if (!handle_quotes(input[i], &quote_flag, output, &j))
        {
            if (quote_flag == 0 && (input[i] == '>' || input[i] == '<'))
                handle_redirection(input, &i, output, &j);
            else
                output[j++] = input[i++];
        }
        else
            i++;
    }
    output[j] = '\0';
    return output;
}
