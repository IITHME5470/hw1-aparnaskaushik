#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to read an n x n matrix from a file
void read_matrix(const char* filename, int n, double** matrix) {
    int i,j;
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening matrix file");
        exit(1);
    }

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            fscanf(file, "%lf,", &matrix[i][j]);
        }
    }

    fclose(file);
}

// Function to read a vector of size n from a file
void read_vector(const char* filename, int n, double* vector) {
    int i;
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening vector file");
        exit(1);
    }

    for (i = 0; i < n; i++) {
        fscanf(file, "%lf,", &vector[i]);
    }

    fclose(file);
}

// Function to write the eigenvalue to a vector file
void append_eigenvalue(const char* filename, double eigenvalue) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Error opening vector file for appending");
        exit(1);
    }

    fprintf(file, "\n%.15f", eigenvalue);
    fclose(file);
}

// Function to check if a vector is an eigenvector and calculate the eigenvalue
int is_eigenvector(int n, double** matrix, double* vector, double* eigenvalue) {
    int i,j;
    double* result = (double*)malloc(n * sizeof(double));
    for (i = 0; i < n; i++) {
        result[i] = 0;
        for (j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    int is_eigenvector = 1;
    *eigenvalue = 0;
    int first_nonzero_index = -1;

    // Find the first non-zero component to calculate the eigenvalue
    for (i = 0; i < n; i++) {
        if (fabs(vector[i]) > 1e-9) {
            *eigenvalue = result[i] / vector[i];
            first_nonzero_index = i;
            break;
        }
    }

    if (first_nonzero_index == -1) {
        // The vector is zero, so it cannot be an eigenvector
        free(result);
        return 0;
    }

    // Verify if A * v == eigenvalue * v
    for (i = 0; i < n; i++) {
        if (fabs(vector[i]) > 1e-9) {
            double expected = (*eigenvalue) * vector[i];
            if (fabs(result[i] - expected) > 1e-9) {
                free(result);
                return 0;
            }
        } else if (fabs(result[i]) > 1e-9) {
            // A * v != 0 where v = 0, so not an eigenvector
            free(result);
            return 0;
        }
    }

    free(result);
    return 1;
}

int main() {
    // Read n from the input file
    FILE* input_file = fopen("input.in", "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    int n;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    // Dynamically allocate memory for the matrix and vectors
    int i;
    double** matrix = (double**)malloc(n * sizeof(double*));
    for (i = 0; i < n; i++) {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }

    char matrix_filename[50];
    sprintf(matrix_filename, "inputfiles/mat_%06d.in", n);
    read_matrix(matrix_filename, n, matrix);

    // Iterate through vector files
    int vecnum = 1;
    while (1) {
        char vector_filename[50];
        sprintf(vector_filename, "inputfiles/vec_%06d_%06d.in", n, vecnum);

        FILE* test_file = fopen(vector_filename, "r");
        if (!test_file) break; // Stop if vector file does not exist
        fclose(test_file);

        double* vector = (double*)malloc(n * sizeof(double));
        read_vector(vector_filename, n, vector);

        double eigenvalue;
        if (is_eigenvector(n, matrix, vector, &eigenvalue)) {
            printf("%s : Yes : %.15f\n", vector_filename, eigenvalue);
            append_eigenvalue(vector_filename, eigenvalue);
        } else {
            printf("%s : Not an eigenvector\n", vector_filename);
        }

        free(vector);
        vecnum++;
    }

    // Free allocated memory
    for (i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
