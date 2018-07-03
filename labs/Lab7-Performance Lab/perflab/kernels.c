/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/*
 * Please fill in the following team struct
 */
team_t team = {
    "mdzz",               /* Team name */

    "MDZZ",               /* First member full name */
    "MDZZ@mdzz.edu",      /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/
 char optimized_rotate_descr[] = "optimized_rotate: Optimized implementation using blocking";
 void optimized_rotate(int dim, pixel *src, pixel *dst)
 {
   int i, j, ii, jj;
   int block_size = 16;
   int disp = dim - 1;

   for (i = 0; i < dim; i += block_size)
     for (j = 0; j < dim; j += block_size)
       for (ii = i; ii < i + block_size; ii++)
         for (jj = j; jj < j + block_size; jj++)
           dst[RIDX(disp-jj, ii, dim)] = src[RIDX(ii, jj, dim)];
 }

/*
 * naive_rotate - The naive baseline version of rotate
 */
 char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
 void naive_rotate(int dim, pixel *src, pixel *dst)
 {
   int i, j;

   for (i = 0; i < dim; i++)
     for (j = 0; j < dim; j++)
       dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
 }

/*
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst)
{
    // naive_rotate(dim, src, dst);
    optimized_rotate(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_rotate_functions()
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);
    add_rotate_function(&optimized_rotate, optimized_rotate_descr);
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
static void initialize_pixel_sum(pixel_sum *sum)
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/*
 * accumulate_sum - Accumulates field values of p in corresponding
 * fields of sum
 */
static void accumulate_sum(pixel_sum *sum, pixel p)
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/*
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum)
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/*
 * avg - Returns averaged pixel value at (i,j)
 */
static pixel avg(int dim, int i, int j, pixel *src)
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++)
	    for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++)
        accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/
 char optimized_smooth_descr[] = "optimized_smooth: Optimized implementation";
 void optimized_smooth(int dim, pixel *src, pixel *dst)
 {

   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // |   Left Top  | Edge 0 | Edge 0 | Edge 0 | Edge 0 | Edge 0 | Edge 0 |   Right Top  |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // |    Edge 1   |        |        |        |        |        |        |    Edge 2    |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // |    Edge 1   |        |        |        |        |        |        |    Edge 2    |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // |    Edge 1   |        |        |        |        |        |        |    Edge 2    |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // |    Edge 1   |        |        |        |        |        |        |    Edge 2    |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // |    Edge 1   |        |        |        |        |        |        |    Edge 2    |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // |    Edge 1   |        |        |        |        |        |        |    Edge 2    |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+
   // | Left Bottom | Edge 3 | Edge 3 | Edge 3 | Edge 3 | Edge 3 | Edge 3 | Right Bottom |
   // +-------------+--------+--------+--------+--------+--------+--------+--------------+

   // Smooth the four corners.
   int curr;

   // Left Top
   dst[0].red   = (src[0].red + src[1].red + src[dim].red + src[dim + 1].red) >> 2;
   dst[0].green = (src[0].green + src[1].green + src[dim].green + src[dim + 1].green) >> 2;
   dst[0].blue  = (src[0].blue + src[1].blue + src[dim].blue + src[dim + 1].blue) >> 2;

   // Right Top
   curr = dim - 1;
   dst[curr].red   = (src[curr].red + src[curr - 1].red + src[curr + dim - 1].red + src[curr + dim].red) >> 2;
   dst[curr].green = (src[curr].green + src[curr - 1].green + src[curr + dim - 1].green + src[curr + dim].green) >> 2;
   dst[curr].blue  = (src[curr].blue + src[curr - 1].blue + src[curr + dim - 1].blue + src[curr + dim].blue) >> 2;

   // Left Bottom
   curr *= dim;
   dst[curr].red   = (src[curr].red + src[curr + 1].red + src[curr - dim].red + src[curr - dim + 1].red) >> 2;
   dst[curr].green = (src[curr].green + src[curr + 1].green + src[curr - dim].green + src[curr - dim + 1].green) >> 2;
   dst[curr].blue  = (src[curr].blue + src[curr + 1].blue + src[curr - dim].blue + src[curr - dim + 1].blue) >> 2;

   // Right Bottom
   curr += dim - 1;
   dst[curr].red   = (src[curr].red + src[curr - 1].red + src[curr - dim].red + src[curr - dim - 1].red) >> 2;
   dst[curr].green = (src[curr].green + src[curr - 1].green + src[curr - dim].green + src[curr - dim - 1].green) >> 2;
   dst[curr].blue  = (src[curr].blue + src[curr - 1].blue + src[curr - dim].blue + src[curr - dim - 1].blue) >> 2;

   // Smooth four edges
   int ii, jj, limit;

   // Edge 0
   limit = dim - 1;
   for (ii = 1; ii < limit; ii++)
   {
     dst[ii].red   = (src[ii].red + src[ii - 1].red + src[ii + 1].red + src[ii + dim].red + src[ii + dim - 1].red + src[ii + dim + 1].red) / 6;
     dst[ii].green = (src[ii].green + src[ii - 1].green + src[ii + 1].green + src[ii + dim].green + src[ii + dim - 1].green + src[ii + dim + 1].green) / 6;
     dst[ii].blue  = (src[ii].blue + src[ii - 1].blue + src[ii + 1].blue + src[ii + dim].blue + src[ii + dim - 1].blue + src[ii + dim + 1].blue) / 6;
   }

   // Edge 3
   limit = dim * dim - 1;
   for (ii = (dim - 1) * dim + 1; ii < limit; ii++)
   {
     dst[ii].red   = (src[ii].red + src[ii - 1].red + src[ii + 1].red + src[ii - dim].red + src[ii - dim - 1].red + src[ii - dim + 1].red) / 6;
     dst[ii].green = (src[ii].green + src[ii - 1].green + src[ii + 1].green + src[ii - dim].green + src[ii - dim - 1].green + src[ii - dim + 1].green) / 6;
     dst[ii].blue  = (src[ii].blue + src[ii - 1].blue + src[ii + 1].blue + src[ii - dim].blue + src[ii - dim - 1].blue + src[ii - dim + 1].blue) / 6;
   }

   // Edge 1
   limit = dim * (dim - 1);
   for (jj = dim; jj < limit; jj += dim)
   {
     dst[jj].red = (src[jj].red + src[jj + 1].red + src[jj - dim].red + src[jj - dim + 1].red + src[jj + dim].red + src[jj + dim + 1].red) / 6;
     dst[jj].green = (src[jj].green + src[jj + 1].green + src[jj - dim].green+ src[jj - dim + 1].green + src[jj + dim].green + src[jj + dim + 1].green) / 6;
     dst[jj].blue = (src[jj].blue + src[jj + 1].blue + src[jj - dim].blue + src[jj - dim + 1].blue + src[jj + dim].blue + src[jj + dim + 1].blue) / 6;
   }

   // Edge 2
   for (jj = 2 * dim - 1 ; jj < limit ; jj += dim)
   {
     dst[jj].red = (src[jj].red + src[jj - 1].red + src[jj - dim].red + src[jj - dim - 1].red + src[jj + dim].red + src[jj + dim - 1].red) / 6;
     dst[jj].green = (src[jj].green + src[jj - 1].green + src[jj - dim].green + src[jj - dim - 1].green + src[jj + dim].green + src[jj + dim - 1].green) / 6;
     dst[jj].blue = (src[jj].blue + src[jj - 1].blue + src[jj - dim].blue + src[jj - dim - 1].blue + src[jj + dim].blue + src[jj + dim - 1].blue) / 6;
   }

   // Remaining pixels
   int i, j;
   for (i = 1 ; i < dim - 1 ; i++) {
     for (j = 1 ; j < dim - 1 ; j++) {
       curr = i * dim + j;
       dst[curr].red = (src[curr].red + src[curr - 1].red + src[curr + 1].red + src[curr - dim].red + src[curr - dim - 1].red + src[curr - dim + 1].red + src[curr + dim].red + src[curr + dim - 1].red + src[curr + dim + 1].red) / 9;
       dst[curr].green = (src[curr].green + src[curr - 1].green + src[curr + 1].green + src[curr - dim].green + src[curr - dim - 1].green + src[curr - dim + 1].green + src[curr + dim].green + src[curr + dim - 1].green + src[curr + dim + 1].green) / 9;
       dst[curr].blue = (src[curr].blue + src[curr - 1].blue + src[curr + 1].blue + src[curr - dim].blue + src[curr - dim - 1].blue + src[curr - dim + 1].blue + src[curr + dim].blue + src[curr + dim - 1].blue + src[curr + dim + 1].blue) / 9;
     }
   }

 }

/*
 * naive_smooth - The naive baseline version of smooth
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst)
{
    int i, j;

    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
        dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth.
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst)
{
    // naive_smooth(dim, src, dst);
    optimized_smooth(dim, src, dst);
}


/*********************************************************************
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&optimized_smooth, optimized_smooth_descr);
    /* ... Register additional test functions here */
}
