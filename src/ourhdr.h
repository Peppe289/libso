/* Our own header, to be included *after* all standard system headers */

#ifndef __ourhdr_h
#define __ourhdr_h

#define _GNU_SOURCE

#include <stdio.h>  /* for convenience */
#include <stdlib.h> /* for convenience */
#include <string.h> /* for convenience */
#include <unistd.h> /* for convenience */
#include <sys/timeb.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h> /* required for some of our prototypes */
#include <sys/stat.h>  /* per i flag della open */
#include <fcntl.h>
#include <time.h>     /* per la funzione time() */
#include <sys/wait.h> //per la funzione waitpid

// definizione macro per pipe
#define WRITE_END 1
#define READ_END 0

#define TRUE -1
#define FALSE 0

#define MAXLINE 4096 /* max line length */

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/* default file access permissions for new files */
#define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
/* default permissions for new directories */

typedef void Sigfunc(int); /* for signal handlers */

/* 4.3BSD Reno <signal.h> doesn't define SIG_ERR */
#if defined(SIG_IGN) && !defined(SIG_ERR)
#define SIG_ERR ((Sigfunc *)-1)
#endif

#define print_err_func(...)                \
    do                                     \
    {                                      \
        fprintf(stderr, "%s: ", __func__); \
        fprintf(stderr, __VA_ARGS__);      \
    } while (0);

#define print_info_func(...)               \
    do                                     \
    {                                      \
        fprintf(stdout, "%s: ", __func__); \
        fprintf(stdout, __VA_ARGS__);      \
    } while (0);

#define print_err(...) fprintf(stderr, __VA_ARGS__);

#define print_info(...) fprintf(stdout, __VA_ARGS__);

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/* prototypes for our own functions */
char *path_alloc(int *); /* {Prog pathalloc} */

void err_dump(const char *, ...); /* {App misc_source} */
void err_msg(const char *, ...);
void err_quit(const char *, ...);
void err_ret(const char *, ...);
void err_sys(const char *, ...);

extern struct timeb g_TStart, g_TStop;
extern struct rusage *g_RStart, *g_RStop;
extern struct rusage *g_RChildrenStart, *g_RChildrenStop;
extern struct rusage *g_RThreadStart, *g_RThreadStop;

void startCounting();
void stopCounting();
void printResourceUsage(int);

double getRealTime();
double getSysTime();
double getUserTime();

double TimeDiffmSec(struct timeb Start, struct timeb Stop);
double TimeDiffuSec(struct timeval Start, struct timeval Stop);

#endif /* __ourhdr_h */
