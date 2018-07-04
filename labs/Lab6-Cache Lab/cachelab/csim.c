/*
 * Updated on Jul 3, 2018 by mdzz
 * All rights reserved
 */

#include "cachelab.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>

/* Cache parameters struct */
typedef struct
{
  int s;                /* 2^s cache sets */
  int b;                /* cacheline block size 2^b bytes */
  int E;                /* number of cachelines per set */
  int S;                /* number of sets, derived from S = 2^s */
  int B;                /* cacheline block size (bytes), derived from B = 2^b */
  char *trace_file;     /* valgrind trace file */
} cache_param_t;

/* Always use a 64-bit variable to hold memory addresses*/
typedef unsigned long long int mem_addr_t;

/* Cache Line struct */
typedef struct
{
  int valid;
  mem_addr_t tag;
  int timestamp;
} line_st;

/* Cache set struct */
typedef struct
{
  line_st *lines;
} cache_set;

/* Cache struct */
typedef struct
{
  cache_set *sets;
} cache_t;

/* global variable */
char * program_name = NULL;

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

/* Missing arguments error. */
void missing_args_error()
{
  printf("%s: Missing required command line argument\n", program_name);
}


/* Missing input file error. */
void missing_file_error(char *file)
{
  printf("%s: No such file or directory\n", file);
}


int main(int argc, char** argv)
{
  // Declare variables
  int opt;
  int help_flag = 0, verbose_flag = 0, s_flag = 0, E_flag = 0, b_flag = 0, t_flag = 0;
  cache_param_t param;
  program_name = argv[0];

  /* Read in command line arguments with getopt()
   *
   * `getopt()` to handle two program options: -n, with no associated value; and
   *   n, with no associated value.
   *   t:, which expects an associated value.
   *
   * `atoi()` is a function in C that converts a string into an integer
   *   numerical representation.
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
        E_flag = 1;
        param.E = atoi(optarg);
        break;

      // Number of block bits (B = 2^b is the block size)
      case 'b':
        b_flag = 1;
        param.b = atoi(optarg);
        break;

      // Name of the valgrind trace to replay
      case 't':
        t_flag = 1;
        param.trace_file = optarg;
        break;

      // No command line arguments, print usage info and exit
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
    exit(EXIT_SUCCESS);
  }

  // exit(0) indicates successful program termination & it is fully portable, While
  // exit(1) (usually) indicates unsucessful termination. However, it's usage is non-portable.

  /* If user missed anyone of these arguments, program will print Usage Info and
   * exit the program.
   */
  if (s_flag == 0 || E_flag == 0 || b_flag == 0 || t_flag == 0)
  {
    missing_args_error();
    csim_help_info();
    exit(EXIT_FAILURE);
  }

  FILE *pFile;                          // pointer to FILE object
  pFile = fopen(param.trace_file, "r");  // open file for reading

  /* fopen() returns NULL when The file doesn't exist */
  if(pFile == NULL)
  {
    missing_file_error(param.trace_file);
    exit(EXIT_FAILURE);
  }

  /* Assign cache parameters */
  param.S = (int) pow(2, param.s);
  param.B = (int) pow(2, param.b);

  // Allocate space for sets and lines
  cache_t cache;
  cache.sets = malloc(param.S * sizeof(cache_set));
  for (int i = 0; i < param.S; i++)
  {
    cache.sets[i].lines = malloc(sizeof(line_st) * param.E);
  }

  /* Counters for verbose_flag */
  int hit_count = 0;
  int miss_count = 0;
  int eviction_count = 0;

  char act;       // L, S, M
  int size;              // size read in from file
  int TSTAMP = 0;        // value for LRU
  int empty = -1;        // index of empty space
  int H = 0;             // is there a hit
  int E = 0;             // is there an eviction
  int toEvict = 0;       // keeps track of what to evict
  mem_addr_t addr;

  // Reading lines like " M 20,1" or "L 19,3"
  while(fscanf(pFile," %c %llx,%d", &act, &addr, &size) > 0)
  {
    if (act != 'I')
    {
      // calculate address tag and set index
      mem_addr_t addr_tag = addr >> (param.s + param.b);
      int tag_size = (64 - (param.s + param.b));
      unsigned long long temp = addr << (tag_size);
      unsigned long long setid = temp >> (tag_size + param.b);

      cache_set set = cache.sets[setid];
      int low = INT_MAX;

      for (int e = 0; e < param.E; e++) {
        if (set.lines[e].valid == 1)
        {
          // Look for hit before eviction candidates
          if (set.lines[e].tag == addr_tag)
          {
            hit_count++;
            H = 1;
            set.lines[e].timestamp = TSTAMP;
            TSTAMP++;
          }
          // Look for oldest for eviction.
          else if (set.lines[e].timestamp < low)
          {
            low = set.lines[e].timestamp;
            toEvict = e;
          }
        }
        // if we haven't yet found an empty, mark one that we found.
        else if(empty == -1)
        {
          empty = e;
        }

      }

      // if we have a miss
      if ( H != 1 )
      {
        miss_count++;

        //if we have an empty line
        if (empty > -1)
        {
          set.lines[empty].valid = 1;
          set.lines[empty].tag = addr_tag;
          set.lines[empty].timestamp = TSTAMP;
          TSTAMP++;
        }
        // if the set is full we need to evict
        else if (empty < 0)
        {
          E = 1;
          set.lines[toEvict].tag = addr_tag;
          set.lines[toEvict].timestamp = TSTAMP;
          TSTAMP++;
          eviction_count++;
        }
      }
      //if the instruction is M, we will always get a hit
      if ( act == 'M' )
      {
        hit_count++;
      }
      //if the -v flag is set print out all debug information
      if (verbose_flag == 1)
      {
        printf( "%c ", act );
        printf( "%llx,%d", addr, size );
        if (H == 1)
        {
          printf("Hit ");
        }
        else if (H != 1)
        {
          printf("Miss ");
        }
        if (E == 1)
        {
          printf("Eviction ");
        }
        printf( "\n" );
      }
      empty = -1;
      H = 0;
      E = 0;

  }
}

  fclose(pFile); // close file when done

  printSummary(hit_count, miss_count, eviction_count);
  return 0;
}
