/* more01.c -version 0.1 of more
 * read and print 24 lines the pause for a few special commands
*/

/* +---> show 24 line from input
 *   +--> print [more?] message
 *   |    Input Enter, SPACE, or q
 *   +--> if Enter, advance one line.
 *        if SPACE
 *        if q --> exit
 * 
*/
/**
 * 缺点：不能立即得到输入，比如输入q之后不按enter不回推出
*/
#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512
void do_more(FILE *);
int see_more();
int main(int ac, char *av[]) {  // 一般写作argc， *argv[], 参数个数个参数字符串
    FILE *fp;
    if (ac == 0) {
        do_more(stdin);
    } else {
        while (--ac) {
            if ((fp = fopen(*++av, "r")) != NULL) {  // fopen只读
                do_more(fp);
                fclose(fp);
            } else {
                exit(1);
            }
        }
    }
    return 0;
}

/**
 * read PAGELEN lines, then call see_more() for further instructions
*/
void do_more(FILE *fp) {
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(), reply;
    while (fgets(line, LINELEN, fp)) {  // fgets最多放linelen个字符到line里面
        if (num_of_lines == PAGELEN) {  /* full screen? */
            reply = see_more();         /* y: ask user */
            if (reply == 0) break;      /* n: done */
            num_of_lines -= reply;      /* reset count */
        }
        if (fputs(line, stdout) == EOF) exit(1); /* show line or die*/
        num_of_lines++;                          /* count it*/
    }
}

/**
 * print message, wait for response, return # of lines to advance
 * q means no, space means yes, CR means one line
*/
int see_more() {
    int c;
    printf("\033[7m more? \033[m");
    while ((c = getchar()) != EOF) { /*get response */
        if (c == 'q') return 0;
        if (c == ' ') return PAGELEN;
        if (c == '\n') return 1;
    }
    return 0;
}