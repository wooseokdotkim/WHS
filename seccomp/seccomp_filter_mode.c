#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <seccomp.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

int main() {
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL); // 기본적으로 모든 시스템 호출 차단

    if (ctx == NULL) {
        perror("seccomp_init");
        exit(EXIT_FAILURE);
    }

    // 허용할 시스템 호출 추가
    // 1. getpid() 시스템 호출 허용
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getpid), 0);

    // 2. write() 시스템 호출 허용 (stdout에 출력하기 위해)
    seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);

    // 필터 적용
    if (seccomp_load(ctx) < 0) {
        perror("seccomp_load");
        exit(EXIT_FAILURE);
    }

    // 허용된 시스템 호출 테스트
    const char *msg1 = "Filter mode enabled. Only getpid() and write() are allowed.\n";
    write(STDOUT_FILENO, msg1, strlen(msg1));

    // getpid() 호출 (허용됨)
    pid_t pid = getpid();
    char pid_msg[64];
    snprintf(pid_msg, sizeof(pid_msg), "getpid() returned: %d\n", pid);
    write(STDOUT_FILENO, pid_msg, strlen(pid_msg));

    // 허용되지 않은 시스템 호출 시도 
    const char *msg2 = "Trying to call getuid()...\n";
    write(STDOUT_FILENO, msg2, strlen(msg2));
    uid_t uid = getuid();  // 프로세스가 종료
    char uid_msg[64];
    snprintf(uid_msg, sizeof(uid_msg), "getuid() returned: %d\n", uid);
    write(STDOUT_FILENO, uid_msg, strlen(uid_msg));

    seccomp_release(ctx);

    return 0;
}

