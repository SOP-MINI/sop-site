/**
 * \brief POSIX message queue capacity example.
 *
 * Process creates new queue fills it with message.
 * Observe blocking/nonblocking behavior of mq_send().
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>

// TODO: Try to change it to some big numbers
#define MSG_MAX 10
#define MSG_SIZE 64

int main() {
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MSG_MAX;

    mq_unlink("/sop_mqueue");

    mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_EXCL | O_WRONLY | O_NONBLOCK, 0600, &attr); // TODO: Try adding O_NONBLOCK here
    if (mq < 0) {
        perror("mq_open()");
        return 1;
    }

    for (int i = 0; i < 100; ++i) {
        printf("Sending message %d ... ", i);
        fflush(stdout);
        if (mq_send(mq, "text", 5, 7) < 0) {
            perror("mq_send()");
            return 1;
        }
        printf("done!\n");
    }

    return 0;
}

