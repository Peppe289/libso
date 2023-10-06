#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define MAXLINE 1024

static void err_doit(int errnoflag, const char *fmt, va_list ap);

void err_ret(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, fmt, ap);
    va_end(ap);
}

void err_sys(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, fmt, ap);
    va_end(ap);
    exit(1);
}

void err_msg(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);
}

void err_quit(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);
    exit(1);
}

static void err_doit(int errnoflag, const char *fmt, va_list ap) {
    int errno_save;
    char buf[MAXLINE];

    errno_save = errno;

    vsnprintf(buf, sizeof(buf), fmt, ap);

    if (errnoflag) {
        int len = strlen(buf);
        snprintf(buf + len, sizeof(buf) - len, ": %s", strerror(errno_save));
    }

    strcat(buf, "\n");
    fputs(buf, stderr);
    fflush(NULL);
}

char *path_alloc(int *size) {
    char *ptr;

    if ((ptr = (char *)malloc((size_t)PATH_MAX)) == NULL)
        err_sys("malloc error for pathname");

    if (size != NULL)
        *size = (int)PATH_MAX;

    return ptr;
}

void path_free(char *ptr) {
    free(ptr);
}
