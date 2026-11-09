#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <liburing.h>
#include <errno.h>

#define BLOCK_SIZE 4096

struct read_op {
    int file_fd;
    int file_index;
    size_t bytes_read;
    size_t total_file_size;
    char buffer[BLOCK_SIZE];
};

static void submit_read(struct read_op *op, struct io_uring *ring) {
    struct io_uring_sqe *sqe = io_uring_get_sqe(ring);

    size_t remaining = op->total_file_size - op->bytes_read;
    size_t to_read = (remaining < BLOCK_SIZE) ? remaining : BLOCK_SIZE;

    io_uring_prep_read(sqe, op->file_fd, op->buffer, to_read, op->bytes_read);
    io_uring_sqe_set_data(sqe, op);
}

int main(int argc, char *argv[]) {
    struct io_uring ring;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2] ...\n", argv[0]);
        return 1;
    }

    int num_files = argc - 1;

    if (io_uring_queue_init(num_files, &ring, 0) < 0) {
        perror("io_uring_queue_init");
        return 1;
    }

    for (int i = 0; i < num_files; i++) {
        const char *filepath = argv[i + 1];
        int fd = open(filepath, O_RDONLY);
        if (fd < 0) {
            perror("open");
            continue;
        }

        struct stat st;
        if (fstat(fd, &st) < 0) {
            perror("fstat");
            close(fd);
            continue;
        }

        struct read_op *op = malloc(sizeof(struct read_op));
        if (!op) {
            perror("malloc");
            close(fd);
            continue;
        }

        op->file_fd = fd;
        op->file_index = i + 1;
        op->bytes_read = 0;
        op->total_file_size = st.st_size;

        if (st.st_size == 0) {
            free(op);
            continue;
        }

        submit_read(op, &ring);
    }

    io_uring_submit(&ring);

    int jobs_remaining = num_files;

    while (jobs_remaining > 0) {
        struct io_uring_cqe *cqe;
        int ret = io_uring_wait_cqe(&ring, &cqe);

        if (ret < 0) {
            perror("io_uring_wait_cqe");
            continue;
        }

        struct read_op *op = io_uring_cqe_get_data(cqe);

        if (cqe->res < 0) {
            fprintf(stderr, "Error reading file %s: %s\n",
                    argv[op->file_index], strerror(-cqe->res));
            goto job_done;
        }

        int bytes_just_read = cqe->res;

        if (bytes_just_read == 0) {
            goto job_done;
        }

        printf("--- Read %d bytes from %s ---\n",
               bytes_just_read, argv[op->file_index]);
        printf("%.*s", bytes_just_read, op->buffer);
        printf("----------------------------------\n\n");

        op->bytes_read += bytes_just_read;

        if (op->bytes_read >= op->total_file_size) {
            goto job_done;
        }

        submit_read(op, &ring);
        io_uring_submit(&ring);
        goto next_cqe;

job_done:
        close(op->file_fd);
        free(op);
        jobs_remaining--;

next_cqe:
        io_uring_cqe_seen(&ring, cqe);
    }

    io_uring_queue_exit(&ring);
    return 0;
}