/**
 * \brief POSIX message queue creation attributes example.
 *
 * Process creates new queue with explicit attributes.
 * Check limits in:
 * /proc/sys/fs/mqueue/msg_max
 * /proc/sys/fs/mqueue/msgsize_max
 *
 * Observe errors when attributes are too big.
 * Observe persistence of queue when mq_unlink is omitted.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>

// TODO: Try to change it to some big numbers
#define MSG_MAX 3
#define MSG_SIZE 64

int main() {
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MSG_MAX;

    mq_unlink("/sop_mqueue"); // TODO: Comment it and try to change creation attributes

    mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_RDWR, 0600, &attr); // TODO: Try adding O_EXCL here
    if (mq < 0) {
        perror("mq_open()");
        return 1;
    }

    mq_getattr(mq, &attr);
    printf("attr.mq_maxmsg = %ld\n", attr.mq_maxmsg);
    printf("attr.mq_msgsize = %ld\n", attr.mq_msgsize);

    if (mq_send(mq, "text", 5, 7) < 0) {
        perror("mq_send()");
        return 1;
    }

    char buf[MSG_SIZE]; // Now the buffer can be small
    unsigned int prio;

    ssize_t ret = mq_receive(mq, buf, sizeof(buf), &prio);
    if (ret < 0) {
        perror("mq_receive()");
        return 1;
    }

    printf("Received message of size %ld (prio: %u): '%s'", ret, prio, buf);

    mq_close(mq);

    return 0;
}

