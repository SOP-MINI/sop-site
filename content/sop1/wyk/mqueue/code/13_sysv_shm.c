/**
 * \brief Basic SYSV shared memory example.
 *
 * \todo Observe output of 'ipcs -m' command
 * \todo Play with ipcmk/ipcrm commands
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define SHM_ID 321
#define SHM_SIZE 1024

int main() {

    printf("My PID = %d\n", getpid());

    key_t key = ftok(".", SHM_ID);
    // TODO: Comment out IPC_* flags, change key to IPC_PRIVATE
    int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0600);
    if (shm_id < 0) {
        perror("shmget()");
        return 1;
    }

    printf("SysV shared memory identifier: %d\n", shm_id);
    // TODO: Observe output of `ipcs -m -i <msg_id>`

    void *ptr = shmat(shm_id, NULL, 0);
    if (ptr == (void *) -1) {
        perror("shmat()");
        return 1;
    }

    printf("Mapped addres = %p\n", ptr);
    getchar();
    // TODO: Observe contents of /proc/[pid]/maps file

    shmdt(ptr);

    return 0;
}
