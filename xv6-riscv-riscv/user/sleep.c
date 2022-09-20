#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(2, "sleep: lack of arg, need one, find zero\n");
        exit(1);
    }
    else if (argc > 2) {
        fprintf(2, "sleep: too many args, need one, find %d\n", argc - 1);
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}