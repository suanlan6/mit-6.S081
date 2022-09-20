#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MAX_FILE_PATH 512

void
find(const char* dest, char* path) {
    char buf[MAX_FILE_PATH], *p;
    int fd;
    struct dirent de;
    struct stat st;
    
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    switch(st.type) {
        case T_DEVICE:
        case T_FILE:
            if (strcmp(de.name, dest) == 0) {
                memmove(p, de.name, strlen(de.name) + 1);
                printf("%s\n", buf);
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                fprintf(2, "ls: path too long\n");
                break;
            }
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    continue;
                if (strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0)
                    continue;
                memmove(p, de.name, strlen(de.name) + 1);
                if (strcmp(dest, p) == 0)
                    printf("%s\n", buf);
                find(dest, buf);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "find: too few args, needs at least 2\n");
        exit(1);
    }
    int i;
    for (i = 1; i < argc; ++i)
        find(argv[i], ".");
    exit(0);
}