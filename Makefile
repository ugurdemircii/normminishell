NAME = minishell

CC = gcc
CFLAGS = -g 


LDFLAGS = -lreadline
SRCS = 	builtin/builtin_cd.c \
        builtin/builtin_echo.c \
        builtin/builtin_env.c \
        builtin/builtin_exit.c \
        builtin/builtin_export.c \
        builtin/builtin_pwd.c \
        builtin/builtin_unset.c \
        builtin/builtin_utils.c \
        builtin/builtin_utils2.c \
        exec/exec_builtin.c \
        exec/exec_utils.c \
        exec/exec_utils2.c \
        exec/exec.c \
        exec/exec2.c \
        exec/redirect.c \
        parse/clear_quotes.c \
        parse/dolar_expansion.c \
        parse/expansion_utils.c \
        parse/heredoc.c \
        parse/minishell.c \
        parse/parse_utils.c \
        parse/parse.c \
		parse/heredoc_utils.c \
		parse/quote_utils.c \
        parse/syntax_check.c \
        parse/token.c \
        signal.c \
        utils/free_utils.c \
        utils/list_utils.c \
        utils/utils.c \
        utils/utils2.c \
        utils/utils3.c \
        utils/utils4.c \
		utils/utils5.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) ./minishell.h $(LDFLAGS)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re
