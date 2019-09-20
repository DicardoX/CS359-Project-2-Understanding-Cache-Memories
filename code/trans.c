//Yunjia Xi 517030910102
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
    int i, j, k, t;
    int var1, var2, var3, var4, var5, var6, var7, var8;

    if(N == 32 && M == 32)
    {
        /*for(i = 0; i < M; i += 8)
            for(j = 0; j < N; j += 8)
                for(k = i; k < N && k < i + 8; k++)     
                    for(t = j; t < M && t < j + 8; t++)
                        B[t][k] = A[k][t];*/
        for(i = 0; i < M; i += 8)
            for ( j = 0; j < N; j += 8)
                for ( k = 0; k < 8; k++)
                {
                    var1 = A[i+k][j];
                    var2 = A[i+k][j+1];
                    var3 = A[i+k][j+2];
                    var4 = A[i+k][j+3];
                    var5 = A[i+k][j+4];
                    var6 = A[i+k][j+5];
                    var7 = A[i+k][j+6];
                    var8 = A[i+k][j+7];

                    B[j][i+k] = var1;
                    B[j+1][i+k] = var2;
                    B[j+2][i+k] = var3;
                    B[j+3][i+k] = var4;
                    B[j+4][i+k] = var5;
                    B[j+5][i+k] = var6;
                    B[j+6][i+k] = var7;
                    B[j+7][i+k] = var8;
                } 
    }
    else if (N == 64 && M == 64)
    {
        /*for(i = 0; i < M; i += 4)
            for ( j = 0; j < N; j += 4)
                for ( k = 0; k < 4; k++)
                {
                    var1 = A[i+k][j];
                    var2 = A[i+k][j+1];
                    var3 = A[i+k][j+2];
                    var4 = A[i+k][j+3];

                    B[j][i+k] = var1;
                    B[j+1][i+k] = var2;
                    B[j+2][i+k] = var3;
                    B[j+3][i+k] = var4;
                }*/
        for (i = 0; i < N; i += 8)
            for(j = 0; j < M; j += 8)
            {
                for ( k = 0; k < 4; k++)
                {
                    //A1
                    var1 = A[i + k][j];
                    var2 = A[i + k][j + 1];
                    var3 = A[i + k][j + 2];
                    var4 = A[i + k][j + 3];
                    //A2
                    var5 = A[i + k][j + 4];
                    var6 = A[i + k][j + 5];
                    var7 = A[i + k][j + 6];
                    var8 = A[i + k][j + 7];
                    // move A1 to B1
                    B[j][i + k] = var1;
                    B[j + 1][i + k] = var2;
                    B[j + 2][i + k] = var3;
                    B[j + 3][i + k] = var4;
                    //move A2 to B2
                    B[j][i + k + 4] = var5;
                    B[j + 1][i + k + 4] = var6;
                    B[j + 2][i + k + 4] = var7;
                    B[j + 3][i + k + 4] = var8;
                }
                for(t = 0; t < 4; t++)
                {
                     //A3
                    var1 = A[i + 4][j + t];
                    var2 = A[i + 5][j + t];
                    var3 = A[i + 6][j + t];
                    var4 = A[i + 7][j + t];
                    //B2
                    var5 = B[j + t][i + 4];
                    var6 = B[j + t][i + 5];
                    var7 = B[j + t][i + 6];
                    var8 = B[j + t][i + 7];

                    // move A3 to B2
                    B[j + t][i + 4] = var1;
                    B[j + t][i + 5] = var2;
                    B[j + t][i + 6] = var3;
                    B[j + t][i + 7] = var4;

                    //move B2 to B3
                    B[j + t + 4][i] = var5;
                    B[j + t + 4][i + 1] = var6;
                    B[j + t + 4][i + 2] = var7;
                    B[j + t + 4][i + 3] = var8;
                    
                    
                }
                for(k = 4; k < 8; k++)
                {
                    //A4
                    var5 = A[i + k][j + 4];
                    var6 = A[i + k][j + 5];
                    var7 = A[i + k][j + 6];
                    var8 = A[i + k][j + 7];
                    //move A4 to B4
                    B[j + 4][i + k] = var5;
                    B[j + 5][i + k] = var6;
                    B[j + 6][i + k] = var7;
                    B[j + 7][i + k] = var8;
                }
            }
                
    }
    else 
    {
        for(i = 0; i < N; i += 16)
            for(j = 0; j < M; j += 16)
                for(k = i; k < N && k < i + 16; k++)     
                    for(t = j; t < M && t < j + 16; t++)
                        B[t][k] = A[k][t];
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
