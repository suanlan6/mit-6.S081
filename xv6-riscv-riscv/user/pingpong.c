#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_PIPE_BUFFER 512

int
main(int argc, char* argv[]) {
    int rd_wr_p[2];
    int rd_wr_c[2];   
    if (pipe(rd_wr_p) == -1) {
        fprintf(2, "pingpong: pipe init failed\n");
        exit(1);
    }
    if (pipe(rd_wr_c) == -1) {
        fprintf(2, "pingpong: pipe init failed\n");
        exit(1);
    }
    int pid;
    char buf[MAX_PIPE_BUFFER];
    int read_stat;
    if ((pid = fork()) > 0) {
        close(rd_wr_p[0]);
        close(rd_wr_c[1]);
        char byte_val[2];
        byte_val[1] = '0';
        write(rd_wr_p[1], byte_val, sizeof(byte_val));
        read_stat = read(rd_wr_c[0], buf, MAX_PIPE_BUFFER);
        if (read_stat == -1) {
            fprintf(2, "pingpong: read error\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(rd_wr_c[0]);
        close(rd_wr_p[1]);
    }
    else if (pid == 0) {
        close(rd_wr_c[0]);
        close(rd_wr_p[1]);
        char byte_val[2];
        read_stat = read(rd_wr_p[0], byte_val, 1);
        if (read_stat == -1) {
            fprintf(2, "pingpong: read error\n");
            exit(1);
        }
        byte_val[1] = '0';
        printf("%d: received ping\n", getpid());
        write(rd_wr_c[1], byte_val, sizeof(byte_val));
        close(rd_wr_p[0]);
        close(rd_wr_c[1]);
    }
    else {
        fprintf(2, "pingpong: unknown fork id\n");
        exit(1);
    }
    exit(0);
}