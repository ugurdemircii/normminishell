#include "../minishell.h"

static void heredoc_sigint_handler(int signo)
{
    (void)signo;
    write(2, "\n", 1);
    free_for_heredoc(NULL);
    exit(130);
}

static void handle_heredoc_input(int pipe_fd, char *limiter, int expand_flag, t_cmds *cmd)
{
    char *line;
	char *expanded;
    
    while (1)
    {
        line = readline("> ");
        if (!line) 
        {
            fprintf(stderr, "minishell: warning: here-document delimited by end-of-file (wanted `%s`)\n", limiter);
            break;
        }
        if (ft_strcmp(line, limiter) == 0) 
        {
            free(line);
            break;
        }
        if (expand_flag) 
        {
            expanded = expand_arg(line, cmd->head_env, 0, cmd);
            free(line);
            line = expanded;
        }
        write(pipe_fd, line, ft_strlen(line));
        write(pipe_fd, "\n", 1);
        free(line);
    }
}
static void run_child_process(int pipe_fd[2], char *limiter, int expand_flag, t_cmds *cmd)
{
    signal(SIGINT, heredoc_sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    close(pipe_fd[0]);
    handle_heredoc_input(pipe_fd[1], limiter, expand_flag, cmd);
    free_env_list(cmd->head_env);
    free_cmds_list(cmd->head);
    close(pipe_fd[1]);
    exit(0);
}
static int open_heredoc_pipe(char *limiter, int expand_flag, t_env *env, t_cmds *cmd)
{
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1) 
    {
        perror("pipe");
        return -1;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return -1;
    }
    if (pid == 0) 
        run_child_process(pipe_fd, limiter, expand_flag, cmd);
    return handle_parent_process(pid, pipe_fd);
}

int setup_heredocs(t_cmds *cmds, t_env *env)
{
    t_cmds *current;
    t_redirect *redir;

    current = cmds;
    while (current) 
    {
        current->head = cmds;
        current->head_env = env;
        redir = current->redirect_list;
        free_for_heredoc(current);
        while (redir) 
        {
            if (redir->type == 4) 
            {
                redir->heredoc_fd = open_heredoc_pipe(redir->file, redir->heredoc_expand_flag,env, current);
                if (redir->heredoc_fd == -1)
                    return -1;
            }
            redir = redir->next;
        }
        current = current->next;
    }
    return 0;
}