#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHILD_COUNT 4

#define QUEUE_NAME_MAX_LEN 32
#define CHILD_NAME_MAX_LEN 32

#define MSG_SIZE 64
#define MAX_MSG_COUNT 4

#define ROUNDS 5

#define ERR(source) \
    (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

mqd_t create_queue(char* name)
{
    struct mq_attr attr = {};
    attr.mq_maxmsg = MAX_MSG_COUNT;
    attr.mq_msgsize = MSG_SIZE;

    mqd_t res = mq_open(name, O_RDWR | O_NONBLOCK | O_CREAT, 0600, &attr);
    if (res == -1)
        ERR("mq_open");

    return res;
}

typedef struct
{
    char* name;
    mqd_t queue;
} ChildData;

void handle_messages(union sigval data);

void register_notification(ChildData* data)
{
    struct sigevent notification = {};
    notification.sigev_value.sival_ptr = data;
    notification.sigev_notify = SIGEV_THREAD;
    notification.sigev_notify_function = handle_messages;

    int res = mq_notify(data->queue, &notification);
    if (res == -1)
        ERR("mq_notify");
}

void handle_messages(union sigval data)
{
    ChildData* child_data = data.sival_ptr;
    char message[MSG_SIZE];

    register_notification(child_data);

    for (;;)
    {
        int res = mq_receive(child_data->queue, message, MSG_SIZE, NULL);
        if (res != -1)
            printf("%s: Accepi \"%s\"\n", child_data->name, message);
        else
        {
            if (errno == EAGAIN)
                break;
            else
                ERR("mq_receive");
        }
    }
}

void close_queues(mqd_t* queues)
{
    for (int i = 0; i < CHILD_COUNT; ++i)
    {
        mq_close(queues[i]);
    }
}

void unlink_queues(char queue_names[CHILD_COUNT][CHILD_NAME_MAX_LEN])
{
    for (int i = 0; i < CHILD_COUNT; ++i)
    {
        int result = mq_unlink(queue_names[i]);
        // The case when the queue does not exist already
        // is not treated as an error
        if (result == -1 && errno != ENOENT)
            ERR("mq_close");
    }
}

void send_message(mqd_t queue, char* content)
{
    int result = mq_send(queue, content, MSG_SIZE, 0);
    if (result == -1)
        ERR("mq_send");
}

void child_function(char* name, mqd_t* queues, int i)
{
    printf("%s: A PID %d incipiens.\n", name, getpid());
    srand(getpid());

    ChildData child_data = {};
    child_data.name = name;
    child_data.queue = queues[i];

    union sigval data;
    data.sival_ptr = &child_data;
    handle_messages(data);  // TODO: dlaczego nie register notification?

    for (int i = 0; i < ROUNDS; ++i)
    {
        int receiver = rand() % CHILD_COUNT;
        char message[MSG_SIZE];
        switch (rand() % 3)
        {
            case 0:
                snprintf(message, MSG_SIZE, "%s: Salve %d!", name, receiver);
                break;
            case 1:
                snprintf(message, MSG_SIZE, "%s: Visne garum emere, %d?", name, receiver);
                break;
            case 2:
                snprintf(message, MSG_SIZE, "%s: Fuistine hodie in thermis, %d?", name, receiver);
                break;
        }

        send_message(queues[receiver], message);

        int sleep_time = rand() % 3 + 1;
        sleep(sleep_time);
    }

    printf("%s: Disceo.\n", name);

    exit(EXIT_SUCCESS);
}

void spawn_child(char* name, mqd_t* queues, int i)
{
    switch (fork())
    {
        case 0:
            child_function(name, queues, i);
        case -1:
            ERR("fork");
    }
}

int main(void)
{
    mqd_t queues[CHILD_COUNT];
    char names[CHILD_COUNT][CHILD_NAME_MAX_LEN];
    char queue_names[CHILD_COUNT][CHILD_NAME_MAX_LEN];

    for (int i = 0; i < CHILD_COUNT; ++i)
    {
        snprintf(queue_names[i], QUEUE_NAME_MAX_LEN, "/child_%d", i);
        queues[i] = create_queue(queue_names[i]);

        snprintf(names[i], CHILD_NAME_MAX_LEN, "Persona %d", i);
    }

    for (int i = 0; i < CHILD_COUNT; ++i)
    {
        spawn_child(names[i], queues, i);
    }

    close_queues(queues);

    while (wait(NULL) > 0)
        ;

    printf("Parens: Disceo.");

    unlink_queues(queue_names);

    return EXIT_SUCCESS;
}
