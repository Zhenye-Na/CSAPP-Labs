/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
  if (M == 32 && N == 32)
  {
    int a, b, c, d;
    for (a = 0 ; a < N ; a += 8) {
      for (b = 0 ; b < M ; b += 8) {
        for (c = b ; c < b + 8 ; ++c) {
          for (d = a + c - b ; d >= a ; --d) {
            B[c][d] = A[d][c];
          }
          for (d = a + c - b + 1 ; d < a + 8 ; ++d) {
            B[c][d] = A[d][c];
          }
        }
      }
    }
  }

  if (M == 64 && N == 64)
  {
    int tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, a, b, c;
    for (a = 0 ; a < N ; a += 8)
    {
      for (b = 0 ; b < M ; b += 8)
      {
        for (c = b ; c < b + 4 ; ++c)
        {
          tmp0 = A[c][a];
          tmp1 = A[c][a + 1];
          tmp2 = A[c][a + 2];
          tmp3 = A[c][a + 3];
          tmp4 = A[c][a + 4];
          tmp5 = A[c][a + 5];
          tmp6 = A[c][a + 6];
          tmp7 = A[c][a + 7];
          B[a][c] = tmp0;
          B[a + 1][c] = tmp1;
          B[a + 2][c] = tmp2;
          B[a + 3][c] = tmp3;
          B[a][c + 4] = tmp4;
          B[a + 1][c + 4] = tmp5;
          B[a + 2][c + 4] = tmp6;
          B[a + 3][c + 4] = tmp7;
        }
        for (c = a + 4 ; c < a + 8 ; ++c)
        {
          tmp0 = B[c - 4][b + 4];
          tmp1 = B[c - 4][b + 5];
          tmp2 = B[c - 4][b + 6];
          tmp3 = B[c - 4][b + 7];

          B[c - 4][b + 4] = A[b + 4][c - 4];
          B[c - 4][b + 5] = A[b + 5][c - 4];
          B[c - 4][b + 6] = A[b + 6][c - 4];
          B[c - 4][b + 7] = A[b + 7][c - 4];

          B[c][b] = tmp0;
          B[c][b + 1] = tmp1;
          B[c][b + 2] = tmp2;
          B[c][b + 3] = tmp3;

          B[c][b + 4] = A[b + 4][c];
          B[c][b + 5] = A[b + 5][c];
          B[c][b + 6] = A[b + 6][c];
          B[c][b + 7] = A[b + 7][c];
        }
      }
    }
  }

  if (M == 61 && N == 67)
  {
    int a, b, c, d;
    for (a = 0 ; a < N ; a += 16)
      for (b = 0 ; b < M ; b += 16)
        for (c = b ; (c < b + 16) && (c < M) ; ++c)
          for (d = a ; (d < a + 16) && (d < N) ; ++d)
            B[c][d] = A[d][c];

  }

}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
  int i, j, tmp;

  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      tmp = A[i][j];
      B[j][i] = tmp;
    }
  }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
