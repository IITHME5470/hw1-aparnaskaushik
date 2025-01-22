#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to write the array to a file
void print_to_file(int n, double** A, int format_flag) {
    char filename[50];
    sprintf(filename, "array_%06d", n);
    int i,j;
    
    if (format_flag == 0) {
        strcat(filename, "_asc.out");
        FILE* file = fopen(filename, "w");
        if (!file) {
            perror("Error opening file");
            return;
        }
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                fprintf(file, "%.15f ", A[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
    } else if (format_flag == 1) {
        strcat(filename, "_bin.out");
        FILE* file = fopen(filename, "wb");
        if (!file) {
            perror("Error opening file");
            return;
        }
        for (i = 0; i < n; i++) {
            fwrite(A[i], sizeof(double), n, file);
        }
        fclose(file);
    }
}

int main() {
    // Read n from the file "input.in"
    FILE* input_file = fopen("input.in", "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    int n;
    int i,j;
    fscanf(input_file, "%d", &n);
    fclose(input_file);

    // Dynamically allocate a 2D array
    double** A = (double**)malloc(n * sizeof(double*));
    for (i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
    }

    // Fill the array with A[i][j] = i + j
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            A[i][j] = i + j;
        }
    }

    // Write the array to files in both formats
    print_to_file(n, A, 0); // ASCII format
    print_to_file(n, A, 1); // Binary format

    // Free the allocated memory
    for (i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);

    printf("Files have been written successfully.\n");
    return 0;
}
