/*
 * Updated on Jul 3, 2018 by mdzz
 * All rights reserved
 */

#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

/* Cache parameters struct */
typedef struct {
    int s;                /* 2^s cache sets */
    int b;                /* cacheline block size 2^b bytes */
    int E;                /* number of cachelines per set */
    int S;                /* number of sets, derived from S = 2^s */
    int B;                /* cacheline block size (bytes), derived from B = 2^b */
    char *trace_file;     /* valgrind trace file */
} cache_param_t;


/* Usage Info */
void csim_help_info()
{
  printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <tracefile>\n");
  printf("Options:\n");
  printf("  -h              Print this help message.\n");
  printf("  -v              Optional verbose flag.\n");
  printf("  -s <num>        Number of set index bits.\n");
  printf("  -E <num>        Number of lines per set.\n");
  printf("  -b <num>        Number of block offset bits.\n");
  printf("  -t <tracefile>  Trace file.\n");
  printf("\nExamples:\n");
  printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
  printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");

}


void missing_args()
{
  printf("%s: Missing required command line argument\n", program_name);
}


/* global variable */
char * program_name = NULL;


int main(int argc, char** argv)
{
  // Declare variables
  int opt;
  int help_flag = 0, verbose_flag = 0, s_flag = 0, E_flag = 0, b_flag = 0, t_flag = 0;

  cache_param_t param;

  program_name = argv[0];

  /* Get command line arguments with getopt()
   *
   * `getopt()` to handle two program options: -n, with no associated value; and
   *   n, with no associated value.
   *   t:, which expects an associated value.
   *
   * `atoi()` is a function in C that converts a string into an integer
   *   numerical representation.
   *
   *
   *
   *
   *
   */
  while(-1 != (opt = getopt(argc, argv, "hvs:E:b:t:")))
  {
    switch (opt)
    {
      // Optional help flag that prints usage info
      case 'h':
        help_flag = 1;
        break;

      // Optional verbose flag that displays trace info
      case 'v':
        verbose_flag = 1;
        break;

      // Number of set index bits (S = 2^s is the number of sets)
      case 's':
        s_flag = 1;
        param.s = atoi(optarg);
        break;

      // Associativity (number of lines per set)
      case 'E':
        E_flag == 1;
        param.E = atoi(optarg);
        break;

      // Number of block bits (B = 2 b is the block size)
      case 'b':
        b_flag = 1;
        param.b = atoi(optarg);
        break;

      // Name of the valgrind trace to replay
      case 't':
        t_flag = 1;
        param.trace_file = optarg;
        break;

      // No command line arguments
      default:
        csim_help_info();
        return 0;
    }

  }

  /* If you set `-h` argument, program will print usage info, then terminate.
   * It dooe not care the remaining arguments right or false.
   */
  if (help_flag == 1)
  {
    csim_help_info();
    exit(1);
  }


  /* If user missed anyone of these arguments, program will print Usage Info and
   * exit the program.
   */
  if (s_flag == 0 || b_flag == 0 || t_flag == 0 || E_flag == 0)
  {
    missing_args();
    csim_help_info();
    exit(1);
  }

  // displays trace info
  if (verbose_flag == 1)
    // display_trace_info()
    pass;





  printSummary(0, 0, 0);
  // printSummary(hit_count, miss_count, eviction_count);
  return 0;
}
