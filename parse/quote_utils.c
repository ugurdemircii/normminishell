#include "../minishell.h"



void go_next_quote(char *input, int *i)
{
	char quote_type;

	quote_type = input[(*i)];
	(*i)++;
	while (input[(*i)] && input[(*i)] != quote_type)
		(*i)++;
	if (input[(*i)] == quote_type)
		(*i)++;
}

char *quote(char *input, int *i, char quote_char)
{
    int start;
    char *str;

    start = *i;
    while (input[(*i)] && !isspace(input[(*i)]))
    {
        if (input[(*i)] == '"' || input[(*i)] == '\'')
            go_next_quote(input,i);
        else
            (*i)++;
    }
    str = ft_substr(input, start, *i - start);
    return (str);
}

char *no_quote(char *input, int *i)
{
    int start;
    char *str;
    int quote_count;
    char quote_type;
    start = *i;
    quote_count = 0;
    while (input[*i])
    {
        if (isspace(input[*i]))
            break;
        if (input[*i] == 34 || input[*i] == 39)
            go_next_quote(input,i);
        else
            (*i)++;
    }
    str = ft_substr(input,start,*i - start);
    return (str);
}

