/* sighand.c - illustrates use of thread masks.
Send SIGINT in different phases of process operation.
Can you explain process behaviour? */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
sigset_t new;
struct sigaction act;
void inthandler(int sig){
	printf("thread %lu caught signal %d\n",(unsigned long)pthread_self(),sig);
}
void *sigthread(void *p){
	pthread_sigmask(SIG_UNBLOCK,&new,NULL);
	printf("Press CTRL-C to deliver SIGINT signal to the process\n");
	while(sleep(4)>0);
	printf("sigthread is done\n");
	return NULL;
}
int main(int argc, char *argv[]){
pthread_t tid;
	act.sa_handler=inthandler;
	sigaction(SIGINT,&act,NULL);
	sigemptyset(&new);
	sigaddset(&new,SIGINT);
	pthread_sigmask(SIG_BLOCK,&new,NULL);
	pthread_create(&tid,NULL,sigthread,NULL);
	pthread_join(tid,NULL);
	printf("thread # %lu has exited\n",(unsigned long)tid);
	sleep(5);
	pthread_sigmask(SIG_UNBLOCK,&new,NULL);
	printf("the main thread can be interrupted\n");
	sleep(6);
	printf("main thread # %lu is done\n",(unsigned long)pthread_self());
	return 0;
}
