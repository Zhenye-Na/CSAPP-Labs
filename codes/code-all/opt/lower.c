/* Benchmark versions of string-lower-casing function */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clock.h"
#include "fcyc.h"

#define LSIZE 500000
#define LINCR  20000

#define ESIZE (1<<20)
#define EMIN (1<<14)

#define ASIZE (LSIZE < ESIZE ? ESIZE : LSIZE)

/* Keep track of a number of different procedures */
#define MAX_BENCHMARKS 10

static char data[ASIZE];

static double clock_rate = 0.0;

static void init()
{
  long i;
  for (i = 0; i < ASIZE; i++)
    data[i] = 'a' + i%26;
}

static void set_len(long len)
{
  data[len] = '\0';  
}

static void unset_len(long len)
{
  data[len] = 'a' + len%26;
}

/* $begin strfuns */
/* Convert string to lowercase: slow */
void lower1(char *s)
{
    long i;

    for (i = 0; i < strlen(s); i++)
	if (s[i] >= 'A' && s[i] <= 'Z')
	    s[i] -= ('A' - 'a');
}

/* Convert string to lowercase: faster */
void lower2(char *s)
{
    long i;
    long len = strlen(s);

    for (i = 0; i < len; i++)
	if (s[i] >= 'A' && s[i] <= 'Z')
	    s[i] -= ('A' - 'a');
}

/* Sample implementation of library function strlen */
/* Compute length of string */
size_t strlen(const char *s)
{
    long length = 0;
    while (*s != '\0') {
	s++; 
	length++;
    }
    return length;
}
/* $end strfuns */

void test_lower1(long *lenp)
{
  long len = *lenp;
  set_len(len);
  lower1(data);
  unset_len(len);
}

void test_lower2(long *lenp)
{
  long len = *lenp;
  set_len(len);
  lower2(data);
  unset_len(len);
}

/* Perform test of function */
static double run_test(test_funct fun, long len)
{
  double cycs;
  double time;
  
  set_fcyc_compensate(0);
  cycs = fcyc(fun,&len);
  time = cycs/(clock_rate*1e6);
  return time;
}



void linear()
{
  long size;
  printf("Length\tlower1\tlower2\n");
  for (size = LINCR; size <= LSIZE; size+=LINCR) {
    printf("%ld", size);
    printf("\t%f", run_test(test_lower1, size));
    printf("\t%f\n", run_test(test_lower2, size));
  }
}

void expo()
{
  long size;
  for (size = EMIN; size <= ESIZE; size += size)
    printf("& %ld ", size);
  printf(" \\\\\n");
  for (size = EMIN; size <= ESIZE; size += size)
    printf("& %.2f ", run_test(test_lower1, size));
  printf(" \\\\\n");
  for (size = EMIN; size <= ESIZE; size += size)
    printf("& %.4f ", run_test(test_lower2, size));
  printf(" \\\\\n");
}

int main(int argc, char *argv[])
{
  init();
  clock_rate = mhz(1);
  if (argc > 1)
    expo();
  else
    linear();
  return 0;
}

