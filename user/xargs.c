#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXARGS 32
#define ARGLENGTH 50

int
main(int argc, char *argv[]) {
    char *args[MAXARGS];
    char buf[512];
    char a[ARGLENGTH];

    int i = 0;
    for (i = 0; i < argc; i++) {
        args[i] = malloc(sizeof(char) * ARGLENGTH);
        strcpy(args[i], argv[i + 1]);
    }

    int argi = argc - 1;
    while (read(0, buf, sizeof(buf)) > 0) {
        char *p;
        int ai = 0;
        for (p = buf; *p; p++) {
            if (argi >= MAXARGS) {
                printf("over max args\n");
                exit();
            }
            if (*p == '\n') {
                a[ai++] = '\0';
                strcpy(args[argi++], a);
                args[argi] = 0;
                if (fork() == 0) {
                    exec(args[0], args);
                } else {
                    wait();
                }
                argi = argc - 1;
                ai = 0;
            } else if (*p == ' ' || *p == '\t') {
                a[ai++] = '\0';
                strcpy(args[argi++], a);
                ai = 0;
            } else {
                a[ai++] = *p;
            }
        }
    }
    exit();
}
