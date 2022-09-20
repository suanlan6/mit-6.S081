#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX_BUF_SIZE 512

int readpipe(char** args, int argc) {
    char bufr[2], buf[MAX_BUF_SIZE];
    int total_num = argc, pos = 0;
    while(read(0, bufr, 1) > 0) {
        if (*bufr == '\n') {
            ++total_num;
            break;
        }
        else {
            buf[pos++] = bufr[0];
            if (bufr[0] == ' ')
                ++total_num;
        }
    }
    buf[pos] = 0;
    if (strlen(buf) >= MAX_BUF_SIZE) {
        fprintf(2, "xargs: args too long\n");
        exit(1);
    }
    strcpy(*args, buf);
    return total_num - argc;
}

int
main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "xargs: too few args ,at least 2\n");
        exit(1);
    }
    char *buf = (char*)malloc(MAX_BUF_SIZE), *args[MAXARG] = {0};
    int i, n;
    for (i = 0; i < argc; ++i)
        strcpy(args[i] = (char*)malloc(MAX_BUF_SIZE), argv[i]);
    while((n = readpipe(&buf, argc)) > 0) {
        if (n > MAXARG) {
            fprintf(2, "xargs: too many args, no more than 32\n");
            exit(1);
        }
        int start = 0, pos = 0, t = argc;
        while(pos <= strlen(buf)) {
            if (buf[pos] == ' ' || buf[pos] == 0) {
                memcpy(args[t] = (char*)malloc(sizeof(buf)), buf + start, pos - start);
                args[t++][pos - start] = 0;
                start = pos + 1;
            }
            if (buf[pos] == 0)
                break;
            ++pos;
        }
        
        if (fork() == 0) {
            exec(args[1], args + 1);
            fprintf(2, "xargs: can not exec %s\n", args[1]);
            exit(1);
        }
        else
            wait(0);
        for (int i = argc; i < n; ++i)
            free(argv[i]);
    }
    for (int i = 0; i < argc; ++i)
        free(args[i]);
    free(buf);
    exit(0);
}