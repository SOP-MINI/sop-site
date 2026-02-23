#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define SHM_NAME "/tug.shm"
#define BOARD_SIZE 50

void run_arbiter() {
    // 1. Clean up any leftover memory from previous crashes
    shm_unlink(SHM_NAME);

    // 2. Create fresh shared memory
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); exit(1); }

    size_t length = BOARD_SIZE * sizeof(int);
    if (ftruncate(fd, length) == -1) { perror("ftruncate"); exit(1); }

    int* board = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (board == MAP_FAILED) { perror("mmap"); exit(1); }
    close(fd);

    // 3. Initialize the board
    memset(board, 0, length);
    board[BOARD_SIZE / 2] = 1;

    printf("Arbiter started. Waiting for players to join...\n");

    // 4. Main Arbiter Loop (Rendering and checking rules)
    while (1) {
        int pos = -1;
        int ones_count = 0;

        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i] == 1) {
                pos = i;
                ones_count++;
            }
        }

        // Render the board
        printf("\r[");
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i] == 1) printf("*");
            else if (i == BOARD_SIZE / 2) printf("|");
            else printf("-");
        }
        printf("] ");
        fflush(stdout);

        // Check for anomalies (Race conditions detected!)
        if (ones_count == 0) {
            printf("\n\n[Arbiter] ERROR: The rope disappeared! (Race condition)\n");
            break;
        }
        if (ones_count > 1) {
            printf("\n\n[Arbiter] ERROR: The rope cloned itself! %d ropes detected!\n", ones_count);
            break;
        }

        // Check win conditions
        if (pos == 0) {
            printf("\n\n[Arbiter] LEFT side wins!\n");
            break;
        } else if (pos == BOARD_SIZE - 1) {
            printf("\n\n[Arbiter] RIGHT side wins!\n");
            break;
        }

        usleep(30000); // 30ms refresh rate
    }

    // 5. Cleanup
    memset(board, 0, length); // Zero out the board to signal players to stop
    munmap(board, length);
    shm_unlink(SHM_NAME);
    printf("Arbiter exiting. Game over.\n");
}

void run_player(int direction, const char* name) {
    // 1. Open EXISTING shared memory (No O_CREAT!)
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open (Is the Arbiter running?)");
        exit(1);
    }

    size_t length = BOARD_SIZE * sizeof(int);
    int* board = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (board == MAP_FAILED) { perror("mmap"); exit(1); }
    close(fd);

    printf("Player %s joined the game!\n", name);
    srand(getpid()); // Seed for random sleep

    // 2. Main Player Loop
    while (1) {
        int pos = -1;

        // Find the rope
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board[i] == 1) pos = i;
        }

        // If rope is missing or at the edges, the game is over
        if (pos <= 0 || pos >= BOARD_SIZE - 1) {
            break;
        }

        int new_pos = pos + direction;

        // Pull the rope (RACE CONDITION DANGER ZONE!)
        board[pos] = 0;
        board[new_pos] = 1;

        // Sleep for a random time (10ms - 30ms) to make race conditions more frequent
        usleep(10000 + (rand() % 20000));
    }

    munmap(board, length);
    printf("Player %s exiting.\n", name);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <arbiter|left|right>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "arbiter") == 0) {
        run_arbiter();
    } else if (strcmp(argv[1], "left") == 0) {
        run_player(-1, "LEFT");
    } else if (strcmp(argv[1], "right") == 0) {
        run_player(1, "RIGHT");
    } else {
        fprintf(stderr, "Unknown role. Use arbiter, left, or right.\n");
        return 1;
    }

    return 0;
}
