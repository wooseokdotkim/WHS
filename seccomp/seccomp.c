#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <seccomp.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>

int main() {
    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_STRICT) < 0) {
        write(STDERR_FILENO, "prctl(PR_SET_SECCOMP) failed\n", 29);
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Strict mode enabled. Only read, write, _exit, and sigreturn are allowed.\n", 72);

    pid_t pid = getpid();    // <- Killed!
    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "getpid() returned: %d\n", pid);
    write(STDOUT_FILENO, buffer, len);
    return 0;
}

