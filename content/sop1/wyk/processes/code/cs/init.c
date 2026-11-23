#include <unistd.h>
#include <sys/prctl.h>

void __attribute__((noinline)) do_parent() {
    prctl(PR_SET_NAME, "parent_init", 0, 0, 0);
    while (1) {
        asm volatile("nop");
    }
}

void __attribute__((noinline)) do_child() {
    prctl(PR_SET_NAME, "child", 0, 0, 0);
    while (1) {
        asm volatile("nop");
    }
}

int main() {
    if (fork() == 0) {
        do_child();
    } else {
        do_parent();
    }
    return 0;
}
