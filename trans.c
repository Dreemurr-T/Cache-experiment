//Haoyuan Tian, 519021911174

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
    int i, j, n, m;
    int tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    if (M == 32)
    {
        for (n = 0; n < N; n += 8)
        {
            for (m = 0; m < M; m += 8)
            {
                for (i = n; i < n + 8; i++)
                {
                    for (j = m; j < m + 8; j++)
                    {
                        if (i != j)
                        {
                            B[j][i] = A[i][j];
                        }
                        else
                        {
                            tmp = A[i][j];
                        }
                    }
                    if (m == n)
                    {
                        B[i][i] = tmp;
                    }
                }
            }
        }
    }
    else if (M == 64)
    {
        for (m = 0; m < 64; m += 8)
        {
            for (n = 0; n < 64; n += 8)
            {
                for (i = n; i < n + 4; ++i)
                {
                    tmp = A[i][m];
                    tmp1 = A[i][m + 1];
                    tmp2 = A[i][m + 2];
                    tmp3 = A[i][m + 3];
                    tmp4 = A[i][m + 4];
                    tmp5 = A[i][m + 5];
                    tmp6 = A[i][m + 6];
                    tmp7 = A[i][m + 7];

                    B[m][i] = tmp;
                    B[m][i + 4] = tmp4;
                    B[m + 1][i] = tmp1;
                    B[m + 1][i + 4] = tmp5;
                    B[m + 2][i] = tmp2;
                    B[m + 2][i + 4] = tmp6;
                    B[m + 3][i] = tmp3;
                    B[m + 3][i + 4] = tmp7;
                }
                for (i = m; i < m + 4; ++i)
                {
                    tmp = B[i][n + 4];
                    tmp1 = B[i][n + 5];
                    tmp2 = B[i][n + 6];
                    tmp3 = B[i][n + 7];
                    tmp4 = A[n + 4][i];
                    tmp5 = A[n + 5][i];
                    tmp6 = A[n + 6][i];
                    tmp7 = A[n + 7][i];


                    B[i][n + 4] = tmp4;
                    B[i][n + 5] = tmp5;
                    B[i][n + 6] = tmp6;
                    B[i][n + 7] = tmp7;
                    B[i + 4][n] = tmp;
                    B[i + 4][n + 1] = tmp1;
                    B[i + 4][n + 2] = tmp2;
                    B[i + 4][n + 3] = tmp3;
                   
                }
                for (i = m + 4; i < m + 8; ++i)
                {
                    tmp = A[n + 4][i];
                    tmp1 = A[n + 5][i];
                    tmp2 = A[n + 6][i];
                    tmp3 = A[n + 7][i];

                    B[i][n + 4] = tmp;
                    B[i][n + 5] = tmp1;
                    B[i][n + 6] = tmp2;
                    B[i][n + 7] = tmp3;
                }
            }
        }
    }
    else
    {
        for (n = 0; n < N; n += 16)
        {
            for (m = 0; m < M; m += 16)
            {
                for (i = n; (i < n + 16) && (i < N); i++)
                {
                    for (j = m; (j < m + 16) && (j < M); j++)
                    {
                        if (i != j)
                        {
                            B[j][i] = A[i][j];
                        }
                        else
                        {
                            tmp = A[i][j];
                        }
                    }
                    if (m == n)
                    {
                        B[i][i] = tmp;
                    }
                }
            }
        }
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

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
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

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
