#include "../minishell.h"

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