#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define CYCLES 5

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_sensors>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_sensors = atoi(argv[1]);

    sem_unlink("/data_ready");
    sem_unlink("/can_overwrite");

    sem_t *data_ready = sem_open("/data_ready", O_CREAT | O_EXCL, 0644, 0);
    sem_t *can_overwrite = sem_open("/can_overwrite", O_CREAT | O_EXCL, 0644, 0);

    if (data_ready == SEM_FAILED || can_overwrite == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    printf("[Aggregator] Started. Expecting %d sensors.\n", num_sensors);

    for (int i = 1; i <= CYCLES; i++) {
        for (int s = 0; s < num_sensors; s++) {
            sem_wait(data_ready);
        }

        float global_min = 1000.0f;
        float global_max = -1000.0f;
        int readings_count = 0;

        for (int s = 0; s <= num_sensors; s++) {
            char filename[32];
            snprintf(filename, sizeof(filename), "sensor_%d.dat", s);

            FILE *file = fopen(filename, "r");
            if (file) {
                float temp;
                while (fscanf(file, "%f", &temp) == 1) {
                    if (temp < global_min) global_min = temp;
                    if (temp > global_max) global_max = temp;
                    readings_count++;
                }
                fclose(file);
            }
        }

        printf("[Aggregator] Cycle %d: Processed %d readings. Min: %.2f, Max: %.2f\n",
               i, readings_count, global_min, global_max);

        for (int s = 0; s < num_sensors; s++) {
            sem_post(can_overwrite);
        }
    }

    sem_close(data_ready);
    sem_close(can_overwrite);
    sem_unlink("/data_ready");
    sem_unlink("/can_overwrite");

    printf("[Aggregator] Finished and cleaned up.\n");
    return 0;
}
