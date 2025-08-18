#include "../minishell.h"


void free_for_heredoc(t_cmds *cmd)
{
    static t_cmds *tmp;

    if (cmd)
    {
        tmp = cmd;
        return ;
    }
    if (!cmd)
    {
        free_env_list(tmp->head_env);
        free_cmds_list(tmp->head);
    }
    return ;
}


int handle_parent_process(pid_t pid, int pipe_fd[2])
{
    int status;
    
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    close(pipe_fd[1]);
    waitpid(pid, &status, 0);
    setup_signals();
    if (WIFEXITED(status) && WEXITSTATUS(status) == 130) 
    {
        close(pipe_fd[0]);
        return -1;
    }
    return pipe_fd[0];
}