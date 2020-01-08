/** who2.c - read /etc/utmp and list info therein
 *         - suppresses empty recodes
 *         - formats time nicely.
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

#define SHOWHOST
void show_time(long);
void show_info(struct utmp *);
int main() {
    struct utmp utbuf;
    int utmpfd; /* read from this descriptor */

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror(UTMP_FILE); /* UTMP_FILE is in utmp.h */
        exit(1);
    }

    while (read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)) {
        show_info(&utbuf);
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
    if (utbufp->ut_type != USER_PROCESS)
        return;
    printf(" %-8.8s", utbufp->ut_user); /* the log name */
    printf(" ");
    printf(" %-8.8s", utbufp->ut_line); /* the tty */
    printf(" ");
    show_time(utbufp->ut_time); /* display time */
    printf(" ");
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0')
        printf("(%s", utbufp->ut_host); /* the host */
#endif
    printf("\n");
}

/**
 * displays time in a format fit for human consumption
 * uses ctime to build a string then picks parts out of it
 * Note: %12.12s prints a string 12 chars wide and LIMITES  
 * it to 12chars
*/
void show_time(long timeval) {
    char *cp; /* to hold address of time */
    /** convert time to string, string looks like 
      * Mon Feb 4 00:46:40 EST 1991
      * 0123456789012345 */
    cp = ctime(&timeval);
    printf("%12.12s", cp + 4); /* pick 12 chars from pos 4 */
}