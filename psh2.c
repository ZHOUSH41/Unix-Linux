/** prompting shell version 2
 * 
 *  Solves the 'one-shot' problem of version 1
 *   Uses execvp(), but fork()s first so that the 
 *   shell waites around to perform another command
 *  New problem: shell catches signals. Run vi, press ^c.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXARGS 20
#define ARGLEN 100

void execute(char *[]);
char *makestring(char *);

int main() {
    char *arglist[MAXARGS + 1];
    int numargs;
    char argbuf[ARGLEN];
    char *makestring();

    numargs = 0;
    while (numargs < MAXARGS) {
        printf("Arg[%d]?", numargs);
        if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n') {
            arglist[numargs++] = makestring(argbuf);
        } else {
            if (numargs > 0) {
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            }
        }
    }
    return 0;
}

void execute(char *arglist[]) {
    /**
     * use fork and execvp and wait to do it
    */
    int pid, exitstatus;

    pid = fork();
    switch (pid) {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            execvp(arglist[0], arglist);
            perror("execvp failed");
            exit(1);
        default:
            while (wait(&exitstatus) != pid)
                ;
            printf("child exited with status %d, %d\n",
                   exitstatus >> 8, exitstatus & 0377);
    }
}

char *makestring(char *buf) {
    char *cp;
    buf[strlen(buf) - 1] = '\0';
    cp = (char *)malloc(strlen(buf) + 1);
    if (cp == NULL) {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}