#include "../minishell.h"

int builtin_pwd(void) 
{
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd))) 
	{
		printf("%s\n", cwd);
			return 0;
	} 
	else 
		perror("pwd");
	return 1;
}