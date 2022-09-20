#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const int max_val = 35;
#define PIPE_LEN sizeof(int)

void next_proc(int pipe1[]) {
    close(pipe1[1]);
    int pid, t, pipe2[2];
    if (read(pipe1[0], &t, PIPE_LEN))
        printf("prime: %d\n", t);
    if (pipe(pipe2) == -1) {
        fprintf(2, "primes: pipe init failed\n");
        exit(1);
    }
    if ((pid = fork()) > 0) {
        close(pipe2[0]);
        int i;
        while(read(pipe1[0], &i, PIPE_LEN)) {
            if (i % t != 0) {
                if (write(pipe2[1], &i, PIPE_LEN) != PIPE_LEN) {
                    fprintf(2, "primes: invalid value\n");
                    exit(1);
                }
            }
        }
        close(pipe1[0]);
        close(pipe2[1]);
        wait((int*)0);
    }
    else if (pid == 0) {
        close(pipe1[0]);
        next_proc(pipe2);
    }
}

int
main(int argc, char* argv[]) {
    int pipe1[2];
    if (pipe(pipe1) == -1) {
        fprintf(2, "primes: pipe init failed\n");
        exit(1);
    }
    int pid;
    if ((pid = fork()) > 0) {
        close(pipe1[0]);
        int i;
        for (i = 2; i < max_val; ++i) {
            if (write(pipe1[1], &i, PIPE_LEN) != PIPE_LEN) {
                fprintf(2, "primes: invalid value\n");
                exit(1);
            }
        }
        close(pipe1[1]);
        wait((int*)0);
    }
    else if (pid == 0)
        next_proc(pipe1);
    else {
        fprintf(2, "primes: unkown fork id\n");
        exit(1);
    }
    exit(0);
}