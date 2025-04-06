#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

typedef int matrix_t;

// To save current row and column for new interrupt handler
matrix_t cur_row;
matrix_t cur_column;
// To save old interrupt
void (*old_int_handler) (int);

// Transpose matrix for better multiplication
void transpose_matrix(matrix_t *matrix, int n)
{
    for(int i = 1; i < n; ++i)
    {
        int offset = i * n;
        for(int j = i + 1; j < n; ++j)
        {
            int mirror_index = j * n + i;
            matrix_t tmp = matrix[offset + j];
            matrix[offset + j] = matrix[mirror_index];
            matrix[mirror_index] = tmp;
        }
    }
}

// Assume matrix B is already transposed
matrix_t mult_row_on_column(matrix_t *A, matrix_t *B, int n, int row, int column)
{
    matrix_t rez = 0;
    for(int i = 0; i < n; ++i)
    {
        rez += A[row * n + i] * B[column * n + i];
    }
    return rez;
}

// Multiply matrix A by matrix B (B is changed after)
void mult(matrix_t *A, matrix_t *B, matrix_t *C, int n)
{
    // Transpose matrixB for better performance
    transpose_matrix(B, n);
    for(int i = 0; i < n; ++i)
    {
        // Save cur_row for signal
        cur_row = i;
        int offset = n * i;
        for(int j = 0; j < n; ++j)
        {
            // Save cur_column for signal
            cur_column = j;
            C[offset + j] = mult_row_on_column(A, B, n, i, j);
            sleep(1);
        }
    }
    // Restore matrix B
    // transpose_matrix(B, n);
}

// Get matrix from file
int get_matrix(const char *filename, matrix_t *matrix, int n)
{
    FILE *file = fopen(filename, "r");
    if(!file)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return 1;
    }
    for(int i = 0; i < n; ++i)
    {
        int offset = n * i;
        for(int j = 0; j < n; ++j)
        {
            if(fscanf(file, "%d", &matrix[offset + j]) < 0)
            {
                fprintf(stderr, "Failed to read matrix from file %s\n", filename);
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

// Dump matrix on the stdout
void print_matrix(matrix_t *matrix, int n)
{
    for(int i = 0; i < n; ++i)
    {
        int offset = n * i;
        for(int j = 0; j < n; ++j)
        {
            printf("%d ", matrix[offset + j]);
        }
        puts("\n");
    }
}

// New interrupt handler
void int_handler(int nsig)
{
    printf("Current row is %d\nCurrent column is %d\n", cur_row, cur_column);
    (void)signal(SIGINT, old_int_handler);
}


int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Too little arguments. Provide filename for matrixes.\n");
        return 1;
    }
    const char *filenameA = argv[1];
    const char *filenameB = argv[2];


    const int MATRIX_SIZE = 1000;
    const int size = MATRIX_SIZE * MATRIX_SIZE;
    matrix_t *matrixA = (matrix_t *)calloc(sizeof(matrix_t), size);
    matrix_t *matrixB = (matrix_t *)calloc(sizeof(matrix_t), size);
    matrix_t *matrixC = (matrix_t *)calloc(sizeof(matrix_t), size);

    if(get_matrix(filenameA, matrixA, MATRIX_SIZE) | get_matrix(filenameB, matrixB, MATRIX_SIZE))
    {
        return 1;
    }

    printf("Matrix A and B are successfully read\n");

    // Changing signal of interrupt and saving the old handler
    old_int_handler = signal(SIGINT, int_handler);

    mult(matrixA, matrixB, matrixC, MATRIX_SIZE);

    print_matrix(matrixC, MATRIX_SIZE);

    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}















