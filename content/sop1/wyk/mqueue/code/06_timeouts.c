/**
 * \brief POSIX message queue timed operations example.
 *
 * Parent opens mqueue and writes several messages to it.
 * Child waits in a receive loop with timeout.
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <sys/wait.h>
#include <time.h>

#define MSG_MAX 10
#define MSG_SIZE 64

int main() {
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MSG_MAX;

    mq_unlink("/sop_mqueue");

    switch (fork()) {
        case -1:
            perror("fork()");
            return 1;
        case 0: {
            // child

            mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_RDONLY, 0600, &attr);
            if (mq < 0) {
                perror("mq_open()");
                return 1;
            }

            char buf[MSG_SIZE];
            unsigned int prio;

            int i = 0;
            while (1) {
                // Calculate when timeout shall occur
                struct timespec ts;
                if (clock_gettime(CLOCK_REALTIME, &ts) < 0) {
                    perror("clock_gettime()");
                    return 1;
                }

                ts.tv_sec += 3; // 3 seconds from now!

                if (mq_timedreceive(mq, buf, sizeof(buf), &prio, &ts) < 0) {
                    if (errno == ETIMEDOUT) {
                        printf("Timeout!\n");
                        mq_close(mq);
                        return 0;
                    }
                    perror("mq_receive()");
                    return 1;
                }
                printf("Received message %d\n", i);
                i++;
            }
        }
        default: {
            // parent

            mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_WRONLY, 0600, &attr);
            if (mq < 0) {
                perror("mq_open()");
                return 1;
            }

            for (int i = 0; i < 5; ++i) {
                printf("Sending message %d\n", i);
                if (mq_send(mq, "text", 5, 7) < 0) {
                    perror("mq_send()");
                    return 1;
                }
                sleep(1);
            }

            mq_close(mq);
            printf("Parent done! Waiting for child...\n");
            while (wait(NULL) > 0);
            break;
        }
    }


    return 0;
}

