/**
 * \brief Basic POSIX message queue creation example.
 *
 * Process creates mqueue with default attributes and inspects them.
 * Compare with contents of:
 * /proc/sys/fs/mqueue/msg_default
 * /proc/sys/fs/mqueue/msgsize_default
 */

#include <unistd.h>
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>

int main() {

    mq_unlink("/sop_mqueue");

    mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_RDWR, 0600, NULL); // TODO: Try to erase last arguments
    if (mq < 0) {
        perror("mq_open()");
        return 1;
    }

    struct mq_attr attr;
    mq_getattr(mq, &attr);
    printf("attr.mq_maxmsg = %ld\n", attr.mq_maxmsg);
    printf("attr.mq_msgsize = %ld\n", attr.mq_msgsize);

    if (mq_send(mq, "text", 5, 3) < 0) {
        perror("mq_send()");
        return 1;
    }

    char *buf = (char *) malloc(attr.mq_msgsize); // Now the buffer may be as big as necessary
    unsigned int prio;

    ssize_t ret = mq_receive(mq, buf, attr.mq_msgsize, &prio);
    if (ret < 0) {
        perror("mq_receive()");
        return 1;
    }

    printf("Received message of size %ld (prio: %u): '%s'", ret, prio, buf);

    free(buf);
    mq_close(mq);

    return 0;
}

