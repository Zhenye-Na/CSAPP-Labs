# Performance Lab

```sh
$ time ./driver
Teamname: mdzz
Member 1: MDZZ
Email 1: MDZZ@mdzz.edu

Rotate: Version = naive_rotate: Naive baseline implementation:
Dim		64	128	256	512	1024	Mean
Your CPEs	2.9	3.9	6.7	10.0	12.1
Baseline CPEs	14.7	40.1	46.4	65.9	94.5
Speedup		5.1	10.4	6.9	6.6	7.8	7.2

Rotate: Version = optimized_rotate: Optimized implementation using blocking:
Dim		64	128	256	512	1024	Mean
Your CPEs	3.0	3.2	4.9	5.1	6.3
Baseline CPEs	14.7	40.1	46.4	65.9	94.5
Speedup		4.9	12.7	9.5	12.9	14.9	10.2

Rotate: Version = rotate: Current working version:
Dim		64	128	256	512	1024	Mean
Your CPEs	3.0	3.1	4.9	5.1	6.4
Baseline CPEs	14.7	40.1	46.4	65.9	94.5
Speedup		4.9	12.8	9.5	12.9	14.8	10.3

Smooth: Version = smooth: Current working version:
Dim		32	64	128	256	512	Mean
Your CPEs	19.0	23.0	21.5	21.7	22.0
Baseline CPEs	695.0	698.0	702.0	717.0	722.0
Speedup		36.5	30.4	32.6	33.1	32.9	33.0

Smooth: Version = naive_smooth: Naive baseline implementation:
Dim		32	64	128	256	512	Mean
Your CPEs	69.9	70.3	70.4	71.4	69.6
Baseline CPEs	695.0	698.0	702.0	717.0	722.0
Speedup		9.9	9.9	10.0	10.0	10.4	10.1

Smooth: Version = optimized_smooth: Optimized implementation:
Dim		32	64	128	256	512	Mean
Your CPEs	19.0	23.0	22.3	21.7	21.9
Baseline CPEs	695.0	698.0	702.0	717.0	722.0
Speedup		36.5	30.3	31.5	33.1	32.9	32.8

Summary of Your Best Scores:
  Rotate: 10.3 (rotate: Current working version)
  Smooth: 33.0 (smooth: Current working version)

real	0m5.037s
user	0m1.955s
sys	0m3.070s
```

## Rotate()

Optimize using blocking to improve usage of L1 cache (Increasing hits).

```c
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
```

**Reference:** 
- [How to Use Loop Blocking to Optimize Memory Use on 32-Bit Intel® Architecture](https://software.intel.com/en-us/articles/how-to-use-loop-blocking-to-optimize-memory-use-on-32-bit-intel-architecture)
- [Using Blocking to Increase Temporal Locality](http://csapp.cs.cmu.edu/2e/waside/waside-blocking.pdf)

## Smooth()

```c
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
```
