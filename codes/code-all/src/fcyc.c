/* Compute time used by function f */
#include <stdlib.h>
#include <sys/times.h>
#include <stdio.h>

#include "clock.h"
#include "fcyc.h"

#define K 3
#define MAXSAMPLES 20
#define EPSILON 0.01 
#define COMPENSATE 0
#define CLEAR_CACHE 0
#define CACHE_BYTES (1<<19)
#define CACHE_BLOCK 32

static long int kbest = K;
static long int compensate = COMPENSATE;
static long int clear_cache = CLEAR_CACHE;
static long int maxsamples = MAXSAMPLES;
static double epsilon = EPSILON;
static long int cache_bytes = CACHE_BYTES;
static long int cache_block = CACHE_BLOCK;

static long int *cache_buf = NULL;

static double *values = NULL;
static long int samplecount = 0;

#define KEEP_VALS 0
#define KEEP_SAMPLES 0

#if KEEP_SAMPLES
static double *samples = NULL;
#endif

/* Start new sampling process */
static void init_sampler()
{
    if (values)
	free(values);
    values = calloc(kbest, sizeof(double));
#if KEEP_SAMPLES
    if (samples)
	free(samples);
    /* Allocate extra for wraparound analysis */
    samples = calloc(maxsamples+kbest, sizeof(double));
#endif
    samplecount = 0;
}

/* Add new sample.  */
static void add_sample(double val)
{
    long int pos = 0;
    if (samplecount < kbest) {
	pos = samplecount;
	values[pos] = val;
    } else if (val < values[kbest-1]) {
	pos = kbest-1;
	values[pos] = val;
    }
#if KEEP_SAMPLES
    samples[samplecount] = val;
#endif
    samplecount++;
    /* Insertion sort */
    while (pos > 0 && values[pos-1] > values[pos]) {
	double temp = values[pos-1];
	values[pos-1] = values[pos];
	values[pos] = temp;
	pos--;
    }
}

/* Have kbest minimum measurements converged within epsilon? */
static long int has_converged()
{
    return
	(samplecount >= kbest) &&
	((1 + epsilon)*values[0] >= values[kbest-1]);
}

/* Code to clear cache */


static volatile long int sink = 0;

static void clear()
{
    long int x = sink;
    long int *cptr, *cend;
    long int incr = cache_block/sizeof(long int);
    if (!cache_buf) {
	cache_buf = malloc(cache_bytes);
	if (!cache_buf) {
	    fprintf(stderr, "Fatal error.  Malloc returned null when trying to clear cache\n");
	    exit(1);
	}
    }
    cptr = (long int *) cache_buf;
    cend = cptr + cache_bytes/sizeof(long int);
    while (cptr < cend) {
	x += *cptr;
	cptr += incr;
    }
    sink = x;
}

double fcyc(test_funct f, long int *params)
{
    double result;
    init_sampler();
    if (compensate) {
	do {
	    double cyc;
	    if (clear_cache)
		clear();
	    start_counter();
	    f(params);
	    cyc = get_counter();
	    if (cyc > 0.0)
		add_sample(cyc);
	} while (!has_converged() && samplecount < maxsamples);
    } else {
	do {
	    double cyc;
	    if (clear_cache)
		clear();
	    start_counter();
	    f(params);
	    cyc = get_counter();
	    if (cyc > 0.0)
		add_sample(cyc);
	} while (!has_converged() && samplecount < maxsamples);
    }
#ifdef DEBUG
    {
	long int i;
	printf(" %ld smallest values: [", kbest);
	for (i = 0; i < kbest; i++)
	    printf("%.0f%s", values[i], i==kbest-1 ? "]\n" : ", ");
    }
#endif
    result = values[0];
#if !KEEP_VALS
    free(values); 
    values = NULL;
#endif
    return result;  
}


/***********************************************************/
/* Set the various parameters used by measurement routines */


/* When set, will run code to clear cache before each measurement 
   Default = 0
*/
void set_fcyc_clear_cache(long int clear)
{
    clear_cache = clear;
}

/* Set size of cache to use when clearing cache 
   Default = 1<<19 (512KB)
*/
void set_fcyc_cache_sizee(long int bytes)
{
    if (bytes != cache_bytes) {
	cache_bytes = bytes;
	if (cache_buf) {
	    free(cache_buf);
	    cache_buf = NULL;
	}
    }
}

/* Set size of cache block 
   Default = 32
*/
void set_fcyc_cache_block(long int bytes) {
    cache_block = bytes;
}


/* When set, will attempt to compensate for timer interrupt overhead 
   Default = 0
*/
void set_fcyc_compensate(long int compensate_arg)
{
    compensate = compensate_arg;
}

/* Value of K in K-best
   Default = 3
*/
void set_fcyc_k(long int k)
{
    kbest = k;
}

/* Maximum number of samples attempting to find K-best within some tolerance.
   When exceeded, just return best sample found.
   Default = 20
*/
void set_fcyc_maxsamples(long int maxsamples_arg)
{
    maxsamples = maxsamples_arg;
}

/* Tolerance required for K-best
   Default = 0.01
*/
void set_fcyc_epsilon(double epsilon_arg)
{
    epsilon = epsilon_arg;
}





