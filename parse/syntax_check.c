#include "../minishell.h"


void find_pipe(char *input, int *i)
{
    while (input[(*i)])
    {
        if (input[*i] == 34 || input[*i] == 39)
            go_next_quote(input,i);
        if (input[*i] == '|')
            break;
        (*i)++;
    }
}

static int pipe_control(char *input)
{
	int i;

	i = 0;
	if (input[0] == '|')
		return (0);
	return (1);
}

static int consecutive_pipe(char *input)
{
	int i;
	char quote_type;

	i = 0;
	while (input[i])
	{
		if (input[i] == 34 || input[i] == 39)
			go_next_quote(input,&i);
		else if (input[i] == '|' )
		{
			i++;
			while (input[i] && input[i] == ' ')
				i++;
			if (input[i] == '|')
				return (0);
		}
		else
			i++;
	}
	return (1);
}

static int	unclosed_quotes(const char *input)
{
	char	quote_type;
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote_type = input[i];
			i++;
			while (input[i] && input[i] != quote_type)
				i++;
			if (!input[i])
				return (1);
		}
		i++;
	}
	return (0);
}

int check_syntax(char *input)
{
	if (unclosed_quotes(input))
	{
		printf("unclosed quote\n");
		return (0);
	}
	if (!consecutive_pipe(input) || !pipe_control(input))
	{
		printf("unexpected token |\n");
		return (0);
	}
	return (1);
}