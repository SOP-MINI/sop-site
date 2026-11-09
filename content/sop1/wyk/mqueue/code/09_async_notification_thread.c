/**
 * \brief POSIX message queue signal notification example.
 *
 * Parent creates mqueue and reads from it upon receiving notification
 * by a deatched thread. Child process sends random-sized batches of messages.
 *
 * \todo: Observe contents of /dev/mqueue/sop_mqueue during program run
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#define MSG_MAX 10
#define MSG_SIZE 64

volatile sig_atomic_t should_exit = 0;

void sigint_handler(int sig) {
    (void)sig;
    should_exit = 1;
}

void thread_routine(union sigval sv) {
    printf("Notification thread starts!\n");

    mqd_t *mq_ptr = (mqd_t *) sv.sival_ptr;

    // Restore notification
    struct sigevent not;
    memset(&not, 0, sizeof(not));
    not.sigev_notify = SIGEV_THREAD;
    not.sigev_notify_function = thread_routine;
    not.sigev_notify_attributes = NULL; // Thread creation attributes
    not.sigev_value.sival_ptr = mq_ptr; // Thread routine argument
    if (mq_notify(*mq_ptr, &not) < 0) {
      perror("mq_notify()");
      exit(1);
    }

    char buf[MSG_SIZE];
    unsigned int prio;

    // Empty the queue
    while (1) {
        ssize_t ret = mq_receive(*mq_ptr, buf, sizeof(buf), &prio);
        if (ret < 0) {
            if (errno == EAGAIN) break;
            perror("mq_receive()");
            exit(1);
        } else {
            printf("Received message (prio: %u): '%s'\n", prio, buf);
        }
    }

}

int main() {

    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_msgsize = MSG_SIZE;
    attr.mq_maxmsg = MSG_MAX;

    mq_unlink("/sop_mqueue");

    mqd_t mq = mq_open("/sop_mqueue", O_CREAT | O_EXCL | O_RDWR | O_NONBLOCK, 0600, &attr);
    if (mq < 0) {
        perror("mq_open()");
        return 1;
    }

    // Setup SIGINT notification
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction()");
        return 1;
    }

    // Setup (first) notification - this is not inherited!
    struct sigevent not;
    memset(&not, 0, sizeof(not));
    not.sigev_notify = SIGEV_THREAD;
    not.sigev_notify_function = thread_routine;
    not.sigev_notify_attributes = NULL; // Thread creation attributes
    not.sigev_value.sival_ptr = &mq; // Thread routine argument
    if (mq_notify(mq, &not) < 0) {
        perror("mq_notify()");
        return 1;
    }

    switch (fork()) {
        case -1:
            perror("fork()");
            return 1;
        case 0: {
            // child

            srand(getpid());

            // Send 5 blocks of messages
            for (int i = 0; i < 5; ++i) {
                int n = rand() % 3 + 1;
                printf("Sending %d messages\n", n);
                for (int j = 0; j < n; ++j) {
                    if (mq_send(mq, "text", 5, 0) < 0) {
                        perror("mq_send()");
                        return 1;
                    }
                }
                sleep(1);
            }

            kill(getppid(), SIGINT);
            break;
        }
        default: {
            // parent

            sigset_t set, oldset;
            sigemptyset(&set);
            sigaddset(&set, SIGUSR1);
            sigaddset(&set, SIGINT);
            if (sigprocmask(SIG_BLOCK, &set, &oldset) < 0) {
                perror("sigprocmask()");
                return 1;
            }

            while (sigsuspend(&oldset) < 0) {
                if (should_exit)
                    break;
            }

            while (wait(NULL) > 0);
            break;
        }
    }

    mq_close(mq);

    return 0;
}
