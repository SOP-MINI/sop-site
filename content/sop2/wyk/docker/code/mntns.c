#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <sched.h>
#include <string.h>
#include <dirent.h>

#define STACK_SIZE (1024 * 1024)

void print_root_inode() {
    struct stat st;
    if (stat("/", &st) == 0) {
        printf("[child] root (/) inode = %lu\n", st.st_ino);
    }
}

int child_work(void *arg) {
    char* new_root = (char*)arg;
    char put_old[256];
    snprintf(put_old, sizeof(put_old), "%s/old_root", new_root);

    printf("[child] pid = %d\n", getpid());

    // Remount root in non-shared mode (prevent modification of host namespace)
    if (mount("none", "/", NULL, MS_REC | MS_PRIVATE, NULL) == -1) {
        perror("mount(PRIVATE)"); return 1;
    }

    // Mount new root filesystem, in the same place where it resides for now
    if (mount(new_root, new_root, "bind", MS_BIND | MS_REC, NULL) == -1) {
        perror("mount(BIND)"); return 1;
    }

    // Make temporary dir to pivot old root filesystem there
    mkdir(put_old, 0777);

    print_root_inode();

    // Swap the root filesystem
    if (syscall(SYS_pivot_root, new_root, put_old) == -1) {
        perror("pivot_root()"); return 1;
    }

    print_root_inode();

    // Set cwd
    chdir("/");

    // Forget about old root
    umount2("/old_root", MNT_DETACH);
    rmdir("/old_root");

    printf("[child] root (/) contents:\n");
    printf("==================================================\n");

    DIR *dir = opendir("/");
    if (dir != NULL) {
        struct dirent *ent;
        int count = 0;
        // Limit to first 10 items to mimic `head` behavior without blocking
        while ((ent = readdir(dir)) != NULL && count < 10) {
            printf("%s\n", ent->d_name);
            count++;
        }
        closedir(dir);
    } else {
        perror("opendir(/)");
    }
    printf("==================================================\n");

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <rootfs>\n", argv[0]);
        return 1;
    }

    printf("[parent] pid = %d\n", getpid());

    char *stack = malloc(STACK_SIZE);
    if (!stack) {
        perror("malloc");
        return 1;
    }

    pid_t child_pid = clone(child_work, stack + STACK_SIZE, CLONE_NEWNS | SIGCHLD, argv[1]);
    if (child_pid == -1) {
        perror("clone()");
        return 1;
    }

    waitpid(child_pid, NULL, 0);

    printf("[parent] child finished!\n");

    free(stack);
    return 0;
}