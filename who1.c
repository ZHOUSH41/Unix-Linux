/** who1.c - a first version of who program
 *           open, read UTMP files, and show results.
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST /* include remote machine on output. */
void show_info(struct utmp *);
int main() {
    struct utmp current_record;
    int utmpfd; /* read from this descriptor */
    int reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE); /* UTMP_FILE is in utmp.h */
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen) {
        show_info(&current_record);
    }
    close(utmpfd);
    return 0; /* went ok */
}

/**
 * show_info()
 * display contents of the utmp strcut in human readable form
 * *note* thes sizes should notebe hardwired;
*/
void show_info(struct utmp *utbufp) {
    printf(" %-8.8s", utbufp->ut_user); /* the log name */
    printf(" ");
    printf(" %-8.8s", utbufp->ut_line); /* the tty */
    printf(" ");
    printf(" % 10d", utbufp->ut_time); /* log time */
    printf(" ");
#ifdef SHOWHOST
    printf("(%s", utbufp->ut_host); /* the host */
#endif
    printf("\n");
}