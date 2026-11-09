/**
 * \brief POSIX message queue priority example.
 *
 * Process sends 10 messages with random priority to itself.
 * Observe descending priority when receiving.
 * Observe non sticking messages.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <limits.h>

#define MSG_MAX 10
#define MSG_SIZE 64

int main() {
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MSG_MAX;

    mq_unlink("/sop_mqueue");

    mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_RDWR, 0600, &attr);
    if (mq < 0) {
        perror("mq_open()");
        return 1;
    }

    srand(getpid());

    for (int i = 0; i < MSG_MAX; ++i) {
        char buf[MSG_SIZE];
        int prio = (rand() % MQ_PRIO_MAX);
        int n = snprintf(buf, sizeof(buf), "text%d", i);
        printf("Sending message '%s' with priority %d\n", buf, prio);
        if (mq_send(mq, buf, n + 1, prio) < 0) {
            perror("mq_send()");
            return 1;
        }
    }

    for (int i = 0; i < MSG_MAX; ++i) {
        char buf[MSG_SIZE];
        unsigned int prio;
        ssize_t ret = mq_receive(mq, buf, sizeof(buf), &prio);
        if (ret < 0) {
            perror("mq_receive()");
            return 1;
        }

        printf("Received message (prio: %u): '%s'\n", prio, buf);
    }

    mq_close(mq);

    return 0;
}
