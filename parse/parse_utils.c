#include "../minishell.h"




int count_arg(char *input)
{
    int i;
    int count;
    
    count = 0;
    i = 0;
    while (input[i])
    {
        while (input[i] && isspace(input[i]))
            i++;
        if (!input[i])
            break;
        count++;
        while (input[i] && !isspace(input[i]))
        {
            if (input[i] == '\'' || input[i] == '"')
                go_next_quote(input, &i);
            else
                i++;
        }
    }
    return count;
}

int  pipe_count(char *input)
{
	int count;
	int i;

	count = 0;
	i = 0;
    while (input[i])
    {
        if (input[i] == 34 || input[i] == 39)
            go_next_quote(input,&i);
        if (input[i] == '|')
			count++;
        i++;
    }
	return (count);
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
    output = ft_calloc(strlen(input) * 3 + 1, sizeof(char));
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
