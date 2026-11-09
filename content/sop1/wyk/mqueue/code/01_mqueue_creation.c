/**
 * \brief Basic POSIX message queue creation example.
 *
 * Process creates mqueue with default attributes.
 * It sends a message to itself via mqueue.
 *
 * Observe persistence in /dev/mqueue
 */

#include <unistd.h>
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>

#define BUF_SIZE 8192

int main() {

    mq_unlink("/sop_mqueue");

    // TODO: Try to erase last arguments, try to modify slashes in queue name
    // TODO: Try to add O_EXCL flag
    mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_RDWR, 0600, NULL);
    if (mq < 0) {
        perror("mq_open()");
        return 1;
    }

    if (mq_send(mq, "text", 5, 3) < 0) {
        perror("mq_send()");
        return 1;
    }

    // TODO: Try to lower the buffer size
    char *buf = (char *) malloc(BUF_SIZE); // Note pretty big buffer here - you may need to adjust it for your system
    unsigned int prio;

    ssize_t ret = mq_receive(mq, buf, BUF_SIZE, &prio);
    if (ret < 0) {
        perror("mq_receive()");
        return 1;
    }

    printf("Received message of size %ld (prio: %u): '%s'", ret, prio, buf);

    free(buf);
    mq_close(mq);

    return 0;
}

