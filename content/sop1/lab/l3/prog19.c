#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096
#define DEFAULT_ARRAYSIZE 10
#define DELETED_ITEM -1
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

typedef struct argsSignalHandler {
	pthread_t tid;
	int *pArrayCount;
	int *array;
	pthread_mutex_t *pmxArray;
	sigset_t *pMask;
	bool *pQuitFlag;
	pthread_mutex_t *pmxQuitFlag;
} argsSignalHandler_t;

void ReadArguments(int argc, char **argv, int *arraySize);
void removeItem(int *array, int *arrayCount, int index);
void printArray(int *array, int arraySize);
void *signal_handling(void *);

int main(int argc, char **argv)
{
	int arraySize, *array;
	bool quitFlag = false;
	pthread_mutex_t mxQuitFlag = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mxArray = PTHREAD_MUTEX_INITIALIZER;
	ReadArguments(argc, argv, &arraySize);
	int arrayCount = arraySize;
	if (NULL == (array = (int *)malloc(sizeof(int) * arraySize)))
		ERR("Malloc error for array!");
	for (int i = 0; i < arraySize; i++)
		array[i] = i + 1;
	sigset_t oldMask, newMask;
	sigemptyset(&newMask);
	sigaddset(&newMask, SIGINT);
	sigaddset(&newMask, SIGQUIT);
	if (pthread_sigmask(SIG_BLOCK, &newMask, &oldMask))
		ERR("SIG_BLOCK error");
	argsSignalHandler_t args;
	args.pArrayCount = &arrayCount;
	args.array = array;
	args.pmxArray = &mxArray;
	args.pMask = &newMask;
	args.pQuitFlag = &quitFlag;
	args.pmxQuitFlag = &mxQuitFlag;
	if (pthread_create(&args.tid, NULL, signal_handling, &args))
		ERR("Couldn't create signal handling thread!");
	while (true) {
		pthread_mutex_lock(&mxQuitFlag);
		if (quitFlag == true) {
			pthread_mutex_unlock(&mxQuitFlag);
			break;
		} else {
			pthread_mutex_unlock(&mxQuitFlag);
			pthread_mutex_lock(&mxArray);
			printArray(array, arraySize);
			pthread_mutex_unlock(&mxArray);
			sleep(1);
		}
	}
	if (pthread_join(args.tid, NULL))
		ERR("Can't join with 'signal handling' thread");
	free(array);
	if (pthread_sigmask(SIG_UNBLOCK, &newMask, &oldMask))
		ERR("SIG_BLOCK error");
	exit(EXIT_SUCCESS);
}

void ReadArguments(int argc, char **argv, int *arraySize)
{
	*arraySize = DEFAULT_ARRAYSIZE;

	if (argc >= 2) {
		*arraySize = atoi(argv[1]);
		if (*arraySize <= 0) {
			printf("Invalid value for 'array size'");
			exit(EXIT_FAILURE);
		}
	}
}

void removeItem(int *array, int *arrayCount, int index)
{
	int curIndex = -1;
	int i = -1;
	while (curIndex != index) {
		i++;
		if (array[i] != DELETED_ITEM)
			curIndex++;
	}
	array[i] = DELETED_ITEM;
	*arrayCount -= 1;
}

void printArray(int *array, int arraySize)
{
	printf("[");
	for (int i = 0; i < arraySize; i++)
		if (array[i] != DELETED_ITEM)
			printf(" %d", array[i]);
	printf(" ]\n");
}

void *signal_handling(void *voidArgs)
{
	argsSignalHandler_t *args = voidArgs;
	int signo;
	srand(time(NULL));
	for (;;) {
		if (sigwait(args->pMask, &signo))
			ERR("sigwait failed.");
		switch (signo) {
		case SIGINT:
			pthread_mutex_lock(args->pmxArray);
			if (*args->pArrayCount > 0)
				removeItem(args->array, args->pArrayCount, rand() % (*args->pArrayCount));
			pthread_mutex_unlock(args->pmxArray);
			break;
		case SIGQUIT:
			pthread_mutex_lock(args->pmxQuitFlag);
			*args->pQuitFlag = true;
			pthread_mutex_unlock(args->pmxQuitFlag);
			return NULL;
		default:
			printf("unexpected signal %d\n", signo);
			exit(1);
		}
	}
	return NULL;
}
