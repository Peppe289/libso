/*
 * resource.c
 *
 *  Created on: 11/apr/2013
 *      Author: user
 */

/*
 * RUSAGE_THREAD non restituisce i dati in modo completo o corretto.
 * Nel caso in cui si volesse usare printResourceUsage(), nel programma con i
 * thread usare RUSAGE_SELF
 */

#include "ourhdr.h"

struct timeb g_TStart, g_TStop;
struct rusage *g_RStart = NULL, *g_RStop = NULL;
struct rusage *g_RChildrenStart = NULL, *g_RChildrenStop = NULL;
struct rusage *g_RThreadStart = NULL, *g_RThreadStop = NULL;
static short countingStarted = FALSE;
static short countingStopped = FALSE;

void startCounting()
{
	countingStarted = TRUE;

	ftime( &g_TStart);

	if (g_RStart == NULL)
		if ((g_RStart = malloc( sizeof(struct rusage))) == NULL)
				return;
	if (g_RChildrenStart == NULL)
		if ((g_RChildrenStart = malloc( sizeof(struct rusage))) == NULL)
				return;
	if (g_RThreadStart == NULL)
			if ((g_RThreadStart = malloc( sizeof(struct rusage))) == NULL)
					return;

	if (getrusage( RUSAGE_SELF, g_RStart) < 0)
		err_ret("Can't get starting values of resource usage");
	if (getrusage( RUSAGE_CHILDREN, g_RChildrenStart) < 0)
		err_ret("Can't get starting values of resource usage");
	if (getrusage( RUSAGE_THREAD, g_RThreadStart) < 0) // @suppress("Symbol is not resolved")
		err_ret("Can't get starting values of resource usage");
}

void stopCounting()
{
	if (countingStarted == TRUE) {
		countingStopped = TRUE;

		ftime( &g_TStop);

		if (g_RStop == NULL)
			if ((g_RStop = malloc( sizeof(struct rusage))) == NULL)
				return;
		if (g_RChildrenStop == NULL)
			if ((g_RChildrenStop = malloc( sizeof(struct rusage))) == NULL)
				return;
		if (g_RThreadStop == NULL)
			if ((g_RThreadStop = malloc( sizeof(struct rusage))) == NULL)
				return;

		if (getrusage( RUSAGE_SELF, g_RStop) < 0)
			err_ret("Can't get final values of resource usage");
		if (getrusage( RUSAGE_CHILDREN, g_RChildrenStop) < 0)
			err_ret("Can't get final values of resource usage");
		if (getrusage( RUSAGE_THREAD, g_RThreadStop) < 0) // @suppress("Symbol is not resolved")
			err_ret("Can't get final values of resource usage");
	}
}

static void printDataFromResource(struct rusage *start,  struct rusage *stop) {
	print_err("\nTotal Elapsed Time: %.3f\n", TimeDiffmSec(g_TStart, g_TStop));
	print_err("Total Sys Time    : %.3f\n", TimeDiffuSec(start->ru_stime, stop->ru_stime));
	print_err("Total User Time   : %.3f\n", TimeDiffuSec(start->ru_utime, stop->ru_utime));
	print_err("Blocking I/O oper.: %ld (in) %ld (out)\n",
				stop->ru_inblock - start->ru_inblock, stop->ru_oublock - start->ru_oublock);

	print_err("Max resident size : %ld\n", stop->ru_maxrss);
	print_err("Integral shared memory size: %ld\n", stop->ru_ixrss);
	print_err("Integral unshared data size: %ld\n", stop->ru_idrss);
	print_err("Integral unshared stack size: %ld\n", stop->ru_isrss);
	print_err("Page reclaims (soft page faults): %ld\n", stop->ru_minflt);
	print_err("Page faults (hard page faults): %ld\n", stop->ru_majflt);
	print_err("# Swap operations: %ld\n\n", stop->ru_nswap);         /* swaps */
}

void printResourceUsage(int who)
{
	if (countingStopped == FALSE)
		stopCounting();

	switch (who)
	{
		case RUSAGE_SELF:
			printDataFromResource(g_RStart, g_RStop);
			break;

		case RUSAGE_CHILDREN:
			printDataFromResource(g_RChildrenStart, g_RChildrenStop);
			break;

		case RUSAGE_THREAD: // @suppress("Symbol is not resolved")
			printDataFromResource(g_RThreadStart, g_RThreadStop);
			break;

		default:
			print_err_func("no instructions found for %d value\n", who);
			break;
	}
}

double getRealTime() {
	if (countingStarted == FALSE || countingStopped == FALSE)
		return 0.;
	else
		return TimeDiffmSec(g_TStart, g_TStop);
}

double getSysTime() {
	if (countingStarted == FALSE || countingStopped == FALSE ||
		g_RStop == NULL || g_RStart == NULL)
		return 0.;
	else
		return TimeDiffuSec(g_RStart->ru_stime, g_RStop->ru_stime);
}

double getUserTime() {
	if (countingStarted == FALSE || countingStopped == FALSE ||
		g_RStop == NULL || g_RStart == NULL)
		return 0.;
	else
		return TimeDiffuSec(g_RStart->ru_utime, g_RStop->ru_utime);
}
double TimeDiffmSec( struct timeb Start, struct timeb Stop)
{
	double diff;

	if (Stop.millitm < Start.millitm) {
		Stop.millitm += 1000;
		Stop.time -= 1;
	}
	diff = (double) (Stop.time - Start.time) + (Stop.millitm - Start.millitm) / 1000.;
	return diff;
}

double TimeDiffuSec( struct timeval Start, struct timeval Stop)
{
	if (Stop.tv_usec < Start.tv_usec) {
		Stop.tv_usec += 1000000;
		Stop.tv_sec -= 1;
	}
	return (double) (Stop.tv_sec - Start.tv_sec + (Stop.tv_usec - Start.tv_usec) / 1000000.);
}

