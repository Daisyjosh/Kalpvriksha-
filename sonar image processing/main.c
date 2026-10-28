// Time Complexity O(n^2)
// Space Complexity O(1)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#define MAX 10

void generate_matrix(int *base,int matrix_size){
    int *pointer = base;

    for(int index = 0; index < matrix_size*matrix_size; index++){
        *pointer = rand() % 256;
        pointer++;
    }
}

void display_matrix(int *base,int matrix_size){
    for(int outer_index=0; outer_index < matrix_size; outer_index++){
        for(int inner_index = 0; inner_index < matrix_size; inner_index++){
            printf("%d ",*(base+outer_index*matrix_size+inner_index));
        }
        printf("\n");
    }
}

void rotate_matrix(int *base,int matrix_size){
    for(int outer_index = 0; outer_index < matrix_size; outer_index++){
        for(int inner_index = outer_index+1; inner_index < matrix_size; inner_index++){
            int *pointer_1 = base + outer_index * matrix_size + inner_index;
            int *pointer_2 = base + inner_index * matrix_size + outer_index;

            int temp = *pointer_1;
            *pointer_1 = *pointer_2;
            *pointer_2 = temp;
        }
    }
    
    for(int outer_index = 0; outer_index < matrix_size; outer_index++){
        for(int inner_index = 0; inner_index < matrix_size/2; inner_index++){
            int *pointer_1 = base + outer_index * matrix_size + inner_index;
            int *pointer_2 = base + outer_index * matrix_size + (matrix_size - 1 - inner_index);

            int temp = *pointer_1;
            *pointer_1 = *pointer_2;
            *pointer_2 = temp;
        }
    }
}

void smooth_matrix(int *base, int matrix_size){
    for(int row_index = 0; row_index < matrix_size; row_index++){
        for(int column_index = 0; column_index < matrix_size; column_index++){
            int neighbor_sum = 0,neighbor_count = 0;

            for(int neighbor_row = row_index - 1; neighbor_row <= row_index + 1; neighbor_row++){
                for(int neighbor_column = column_index - 1; neighbor_column <= column_index + 1; neighbor_column++){
                    if(neighbor_row >= 0 && neighbor_row < matrix_size && neighbor_column >= 0 && neighbor_column < matrix_size){
                        int *neighbor = base + neighbor_row * matrix_size + neighbor_column;
                        neighbor_sum += *neighbor & 255;
                        neighbor_count++;
                    }
                }
            }

            int *current_cell = base + row_index * matrix_size + column_index;
            int average = neighbor_sum / neighbor_count;
            *current_cell = (*current_cell & 255) | (average << 8);
        }
    }

    for(int index = 0; index < matrix_size * matrix_size; index++){
        int *current_cell = base + index;
        *current_cell = (*current_cell >> 8) & 255;
    }
}

int main(void){
    int matrix_size;
    
    int matrix[MAX][MAX];

    int *base = &matrix[0][0];

    char input[100];
    char *endptr;
    long value;

    printf("Enter matrix size (2-10): ");

    if(fgets(input, sizeof(input), stdin) == NULL){
        printf("Error: Invalid matrix size.\n");
        return 1;
    }

    if(input[0] == ' ' || input[0] == '\t' || input[0] == '\n'){
        printf("Error: Invalid matrix size.\n");
        return 1;
    }

    errno = 0;
    value = strtol(input, &endptr, 10);

    if(endptr == input || errno == ERANGE || value < 2 || value > 10){
        printf("Error: Invalid matrix size.\n");
        return 1;
    }

    while(isspace((unsigned char)*endptr)){
        endptr++;
    }

    if(*endptr != '\0'){
        printf("Error: Invalid matrix size.\n");
        return 1;
    }

    matrix_size = (int)value;

    srand((unsigned int)time(NULL));

    generate_matrix(base,matrix_size);
    printf("\nOriginal Randomly Generated Matrix:\n");
    display_matrix(base,matrix_size);

    rotate_matrix(base,matrix_size);

    printf("\nMatrix after 90-degree Clockwise Rotation:\n");
    display_matrix(base,matrix_size);

    smooth_matrix(base,matrix_size);

    printf("\nMatrix after Applying 3*3 Smoothing Filter:\n");
    display_matrix(base,matrix_size);

    return 0;
}