// Time Complexity: O(N)
// Space Complexity: O(N)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct Student{
    int roll_no;
    char name[100];
    int marks[3];
};

int is_integer(char str[]){
    int char_index = 0;

    if (str[0] == '\0')
        return 0;

    while (str[char_index] != '\0') {
        if (!isdigit((unsigned char)str[char_index]))
            return 0;
        char_index++;
    }

    return 1;
}

int is_valid_name(char str[]){
    int char_index = 0;

    if (str[0] == '\0')
        return 0;

    while (str[char_index] != '\0') {
        if (!isalpha((unsigned char)str[char_index]))
            return 0;
        char_index++;
    }

    return 1;
}

int calculate_total(int mark1, int mark2, int mark3){

    if (!((mark1 >= 0) && (mark1 <= 100)) ||
        !((mark2 >= 0) && (mark2 <= 100)) ||
        !((mark3 >= 0) && (mark3 <= 100))) {

        printf("Error: Invalid Input.\n");
        return -1;
    }
    else {
        return mark1 + mark2 + mark3;
    }
}

float calculate_average(int total){
    return (float)total / 3;
}

char calculate_grade(float average){

    if (average >= 85) {
        return 'A';
    }
    else if (average >= 70) {
        return 'B';
    }
    else if (average >= 50) {
        return 'C';
    }
    else if (average >= 35) {
        return 'D';
    }
    else {
        return 'F';
    }
}

void calculate_performance(char grade){

    if (grade == 'A') {
        printf("Performance: *****\n");
    }
    else if (grade == 'B') {
        printf("Performance: ****\n");
    }
    else if (grade == 'C') {
        printf("Performance: ***\n");
    }
    else if (grade == 'D') {
        printf("Performance: **\n");
    }
}

void print_roll_numbers(int roll_numbers[], int roll_index, int total_students){

    if (roll_index == total_students){
        return;
    }

    printf("%d", roll_numbers[roll_index]);

    if (roll_index < total_students - 1){
        printf(" ");
    }

    print_roll_numbers(roll_numbers, roll_index + 1, total_students);
}

int main(){

    int total_students;
    scanf("%d", &total_students);

    if (total_students <= 0 || total_students > 100){
        printf("Error: Invalid Input.");
        return 0;
    }

    int roll_numbers[100];
    int totals[100];
    float averages[100];
    char grades[100];
    struct Student students[100];

    getchar();

    int student_count = 0;

    for (int student_index = 0; student_index < total_students; student_index++){

        char line[200];
        char *tokens[50];
        int token_count = 0;

        struct Student student;

        if (fgets(line, sizeof(line), stdin) == NULL){
            printf("Error: Invalid Input.\n");
            return 0;
        }

        char *token = strtok(line, " \n");

        while (token != NULL){
            tokens[token_count] = token;
            token_count++;
            token = strtok(NULL, " \n");
        }

        if (token_count < 5){
            printf("Invalid input\n");
            return 0;
        }

        if (!is_integer(tokens[0])){
            printf("Error: Invalid Input.\n");
            return 0;
        }

        student.roll_no = atoi(tokens[0]);

        int marks_start_index = token_count - 3;

        if (!is_integer(tokens[marks_start_index]) ||
            !is_integer(tokens[marks_start_index + 1]) ||
            !is_integer(tokens[marks_start_index + 2])){

            printf("Invalid input: marks must be integers\n");
            return 0;
        }

        student.marks[0] = atoi(tokens[marks_start_index]);
        student.marks[1] = atoi(tokens[marks_start_index + 1]);
        student.marks[2] = atoi(tokens[marks_start_index + 2]);

        int total = calculate_total(
            student.marks[0],
            student.marks[1],
            student.marks[2]
        );

        if (total == -1){
            printf("Invalid Input");
            return 0;
        }

        float average = calculate_average(total);
        char grade = calculate_grade(average);

        student.name[0] = '\0';

        for (int token_index = 1; token_index < marks_start_index; token_index++){

            if (!is_valid_name(tokens[token_index])){
                printf("Error: Invalid Input.\n");
                return 0;
            }

            strcat(student.name, tokens[token_index]);

            if (token_index < marks_start_index - 1){
                strcat(student.name, " ");
            }
        }

        int name_length = strlen(student.name);

        if (name_length <= 0 || name_length >= 100){
            printf("Error: Invalid Input");
            return 0;
        }

        students[student_count] = student;
        totals[student_count] = total;
        averages[student_count] = average;
        grades[student_count] = grade;
        roll_numbers[student_count] = student.roll_no;

        student_count++;
    }

    for (int student_index = 0; student_index < student_count; student_index++){

        printf("Roll: %d\n", students[student_index].roll_no);
        printf("Name: %s\n", students[student_index].name);
        printf("Total: %d\n", totals[student_index]);
        printf("Average: %.2f\n", averages[student_index]);
        printf("Grade: %c\n", grades[student_index]);

        if (grades[student_index] == 'F'){
            continue;
        }

        calculate_performance(grades[student_index]);
        printf("\n");
    }

    printf("List of Roll Numbers (via recursion): ");

    print_roll_numbers(roll_numbers, 0, student_count);

    printf("\n");

    return 0;
}