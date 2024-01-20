#include "kernel/types.h"
#include "user/user.h"

#define NPROC 20
#define NSEMLOOPS 60

int testsem = 0;
int donesem = 1;

void
function(int num)
{
    int i;
    sem_wait(testsem);
    printf("Proc %d:", num);
    for(i=0; i<NSEMLOOPS; i++)
        printf("%d", num);
    printf("\n");
    sem_signal(donesem);
    exit(0);
}

int
main(int argc, char *argv[])
{
    int i, pid;
    sem_init(testsem, 2);
    sem_init(donesem, 0);
    printf("Starting semaphore test...\n");
    printf("If this hangs, it's broken: ");
    sem_wait(testsem);
    sem_wait(testsem);
    printf("ok\n");
    for(i=0; i<NPROC; i++) {
        pid = fork();
        if (pid < 0) {
            printf("semtest: fork failed");
            exit(1);
        }
        if (pid == 0)
            function(i);
    }
    for(i=0; i<NPROC; i++) {
        sem_signal(testsem);
        sem_wait(donesem);
    }
    sem_signal(testsem);
    sem_signal(testsem);
    printf("Semaphore test done\n");
    return 0;
}