#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>

// Simple database record
typedef struct {
    uint32_t id;
    char name[28];
    double balance;
} Record;

#define NUM_RECORDS 5

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s <filepath>\n", argv[0]);
        return 1;
    }
    const char* filepath = argv[1];

    // 1. Open file (create if it doesn't exist)
    int fd = open(filepath, O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    ssize_t length = sizeof(Record) * NUM_RECORDS;
    struct stat sb;
    fstat(fd, &sb);

    // 2. Resize if needed
    if (sb.st_size < length)
    {
        printf("Truncating to %lu bytes...\n", length);
        if (ftruncate(fd, length) == -1)
        {
            perror("ftruncate");
            close(fd);
            return 1;
        }
    }

    // 3. Map the file directly to an array of structs
    Record* db = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (db == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return 1;
    }

    close(fd);

    // 4. Initialize if fresh run
    if (sb.st_size == 0)
    {
        printf("Initializing new database...\n");
        for (int i = 0; i < NUM_RECORDS; i++)
        {
            db[i].id = i + 1;
            snprintf(db[i].name, sizeof(db[i].name), "User_%d", i + 1);
            db[i].balance = 100.0 * (i + 1);
        }
    }

    int idx;
    printf("User IDX > ");
    fflush(stdout);
    scanf("%d", &idx);

    // 5. Operate on the data as a normal array
    printf("db[idx]: id=%u, name='%s', balance=%.2f\n",
           db[idx].id, db[idx].name, db[idx].balance);

    printf("Bonus > ");

    // Give User_3 a bonus
    db[idx].balance += 50.5;

    printf("db[idx]: id=%u, name='%s', balance=%.2f\n",
           db[idx].id, db[idx].name, db[idx].balance);

    // TODO: Try to access db[10]. What happens?

    // TODO: Can we share this binary data with other programs?

    munmap(db, length);
    return 0;
}
