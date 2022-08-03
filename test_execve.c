#include "header/pipex.h"

int	main(void)
{
	char *path_cmd1 = "/bin/grep";
	char *args1[3] = {"grep", "file", NULL};
	char *path_cmd2 = "/bin/cat";
	char *args2[2] = {"cat", NULL};
	int fd_pipe[2];
	int	id;
	
	int	fd = open("file1", O_RDWR, 00777);
	pipe(fd_pipe);
	id = fork();
	if (id != 0){
		close(fd_pipe[1]);
		wait(0);
	}
	else{
		dup2(fd, 0);
		dup2(fd_pipe[1], 1);
		execve(path_cmd1, args1, NULL);
	}
	id = fork();
	if (id != 0)
		wait(0);
	else{
		dup2(fd_pipe[0], 0);
		execve(path_cmd2, args2, NULL);
	}
	return (0);
}