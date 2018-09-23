/*check child process's status, restart it when find it exit*/

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

/*example, modify by yourself*/
char *child_process = "./mutiply";
char *args[1] = {NULL};

int main(int argc, char **argv)
{
	pid_t pid;
	args[0] = child_process;

	while(1)
	{
		pid = fork();
		if(pid < 0)
		{
			fprintf(stderr, "fork failed\n");
			continue;
		}
		else if(pid > 0)//father
		{
			//printf("child's pid: [%d]\n", pid);
			pid = wait(NULL);
			//printf("child process %d exit\n", pid);
			
		}
		else//child
		{
			pid_t ppid = getppid();
			//printf("father's pid: [%d]\n", ppid);
			int ret = execve(child_process, args, NULL);
			if(ret < 0)
			{
				fprintf(stderr, "execve failed.\n");
				exit(1);	
			}
			exit(0);
		}
	}	
	return 0;
} 