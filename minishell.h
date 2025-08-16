#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <fcntl.h>

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

extern volatile sig_atomic_t g_exit_status_shell;

typedef struct s_expand 
{
    char *result;
    int i;
    int j;
    bool in_single;
    bool in_double;
} t_expand;

typedef struct s_redirect
{
    char    *file;
    int   type;
    int heredoc_expand_flag;
    int heredoc_fd;
    struct s_redirect *next;
} t_redirect;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next; 
} t_env;

typedef struct s_cmds
{
    char **command;
    int   *token_type;
    char **envp;
    int   expand;
    int exit_status;
    t_env *head_env;
    struct s_cmds *head;
    t_redirect  *redirect_list;
    struct s_cmds *next;
}   t_cmds;


//utils2
char	*ft_strjoin_env(const char *s1, const char *s2, char sep);
int	ft_isalnum(int c);
int	ft_strcmp(const char *s1, const char *s2);
int	ft_isalpha(int c);
int	ft_isspace(int c);
//utils
int	ft_strlen(char *s);
char	*ft_strdup(char *s1);
char	*ft_substr(char *s, unsigned int start, size_t len);
size_t	ft_strlcpy(char *dst, char *src, size_t dstsize);
char	*ft_strcpy(char *dst, const char *src);
//utils3
char	*ft_itoa(int n);
void	*ft_calloc(size_t count, size_t size);
int is_last_index_pipe(char *input);
//utils4
char	*ft_strtrim(char const *s1, char const *set);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int c);
void free_double_ptr(char **args);
int count_len_double_ptr(char **args);
//list_utils
void add_env_node(t_env **head, t_env *new_node);
t_env *create_env_node(char *key, char *value);
void add_cmd_node(t_cmds **head, t_cmds *new_node);
t_redirect	*create_new_redirect(int type, char *file);
void	add_redirect_node(t_redirect **list, t_redirect *new_redirect);
// free_utils
void free_env_list(t_env *env);
void free_redirect_list(t_redirect *redirect);
void free_cmds_list(t_cmds *cmd);
void free_double_ptr(char **args);
//signal
void	ignore_signals(void);
void	setup_exec_signals(void);
void	setup_signals(void);
//syntax_check
int check_syntax(char *input);
//parse
int parse_steps(char *input,t_cmds **cmd, t_env *env,int exit_status);
//parse_utils
char *re_write_redirect(char *input);
char    **pipe_split(char *input);
t_cmds *minishell_split(char *input);
//token
void token_arg(t_cmds *cmd);
int	redirect_token_check(t_cmds *cmd, char *input);
void redirect_and_command(t_cmds *cmd);
void clean_command(t_cmds *cmd);
//dolarexpansion
void expand_cmd(t_cmds *cmd, t_env *env_list);
char *expand_arg(const char *arg, t_env *env_list, int num,t_cmds *cmd);
//expansionutils
void	handle_exit_code(t_expand *expand_info, int exit_status);
void handle_quote(t_expand *expand_info, char c);
char *find_in_env_list(t_env *env_list, char *var_name);
bool is_valid_var_char(char c);
//clearquote
void clear_quotes_of_cmd(t_cmds *cmd);
//heredoc
int setup_heredocs(t_cmds *cmds, t_env *env);
//exec
void execute_command(t_cmds *cmd, t_env *env);
//executils
char **env_to_envp(t_env *env);
//execbuiltin
int is_builtin(char *cmd);
int handle_single_builtin(t_cmds *cmd, t_env **env, char **envp);
//redirect
int setup_redirects(t_redirect *redir);
//builtin
int	builtin_echo(char **args, int exit_after);
#endif