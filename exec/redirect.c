#include "../minishell.h"

void    restore_stdio(int stdin_backup, int stdout_backup)
{
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
}
static int redirect_input(t_redirect *redir) 
{
	int fd = open(redir->file, O_RDONLY);
	if (fd < 0)
		return (perror("open input"), -1);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (perror("dup2 input"), close(fd), -1);
	return (close(fd), 0);
}

static int redirect_output(t_redirect *redir, int append) 
{
	int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
	int fd = open(redir->file, flags, 0644);
	if (fd < 0)
		return (perror("open output"), -1);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (perror("dup2 output"), close(fd), -1);
	return (close(fd), 0);
}

static int for_heredoc_type(t_redirect *redir)
{		
	if (redir->heredoc_fd < 0)
		return -1;
	if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0) 
	{
		perror("dup2 heredoc");
		close(redir->heredoc_fd);
		return -1;
	}
	close(redir->heredoc_fd);
	return (0);
}

int setup_redirects(t_redirect *redir) 
{
	while (redir) 
	{
		if (redir->type == 1) 
		{ 
			if (redirect_output(redir, 0) < 0)
				return -1;
		}
		else if (redir->type == 2) 
		{
			if (redirect_output(redir, 1) < 0)
				return -1;
		}
		else if (redir->type == 3) 
		{
			if (redirect_input(redir) < 0)
				return -1;
		}
		else if (redir->type == 4)
		{
			if (for_heredoc_type(redir)==-1)
				return -1;	
		}
		redir = redir->next;
	}
	return 0;
}