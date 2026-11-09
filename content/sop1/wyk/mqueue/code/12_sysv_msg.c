/**
 * \brief Basic SYSV message queue example.
 *
 * \todo Observe output of 'ipcs -q' command
 * \todo Play with ipcmk/ipcrm commands
 * \todo Check files /proc/sys/kernel/msg***
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define MSG_ID 123

struct message {
    long type; // Required by msgsnd first field
    char txt[32];
};

int main() {

    key_t key = ftok(".", MSG_ID);
    // TODO: Comment out IPC_* flags, change key to IPC_PRIVATE
    int msg_id = msgget(key, IPC_CREAT | 0600);
    if (msg_id < 0) {
        perror("msgget()");
        return 1;
    }

    printf("SysV message queue identifier: %d\n", msg_id);
    // TODO: Observe output of `ipcs -q -i <msg_id>`

    struct message m;
    memset(&m, 0, sizeof(m));
    m.type = 10;
    strncpy(m.txt, "Nice :)", sizeof(m.txt));
    if (msgsnd(msg_id, &m, sizeof(m.txt), 0) < 0) { // Note only data part size here
        perror("msgsnd()");
        return 1;
    }

    // TODO: Play with 4'th argument - message type
    if (msgrcv(msg_id, &m, sizeof(m.txt), 0, 0) < 0) {
        perror("msgrcv()");
        return 1;
    }

    printf("Received message of type %ld: %s\n", m.type, m.txt);

    return 0;
}
