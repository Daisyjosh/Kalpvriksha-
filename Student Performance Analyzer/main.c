#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

// Time Complexity: O(N)
// Space Complexity: O(N)

#define MAX_STUDENTS 100
#define MAX_NAME 300
#define MAX_LINE 1024
#define MAX_TOKENS 100

struct Student{
    int roll_no;
    char name[MAX_NAME];
    int marks[3];
};

int is_integer(const char str[]){
    int character_index = 0;

    if (str[0] == '\0'){
        return 0;
    }

    while (str[character_index] != '\0'){
        if (!isdigit((unsigned char)str[character_index])){
            return 0;
        }
        character_index++;
    }

    return 1;
}

int parse_roll_number(const char str[], int *roll_no){
    char *end;
    errno = 0;

    if (!is_integer(str)){
        return 0;
    }

    long value = strtol(str, &end, 10);

    if (errno == ERANGE || *end != '\0' ||value < 1 || value > INT_MAX){
        return 0;
    }

    *roll_no = (int)value;
    return 1;
}

int is_valid_name(const char str[]){
    int character_index = 0;

    if (str[0] == '\0'){
        return 0;
    }

    while (str[character_index] != '\0'){
        if (!isalpha((unsigned char)str[character_index])){
            return 0;
        }
        character_index++;
    }

    return 1;
}

int parse_mark(const char str[], int *mark){
    char *end;
    errno = 0;

    if (!is_integer(str)){
        return 0;
    }

    long value = strtol(str, &end, 10);

    if (errno == ERANGE || *end != '\0' || value < 0 || value > 100){
        return 0;
    }

    *mark = (int)value;
    return 1;
}

int calculate_total(int mark1, int mark2, int mark3){
    if (mark1 < 0 || mark1 > 100 || mark2 < 0 || mark2 > 100 ||mark3 < 0 || mark3 > 100){
        return -1;
    }

    return mark1 + mark2 + mark3;
}

float calculate_average(int total){
    return (float)total / 3;
}

char calculate_grade(float average){
    if (average >= 85){
        return 'A';
    }
    else if (average >= 70){
        return 'B';
    }
    else if (average >= 50){
        return 'C';
    }
    else if (average >= 35){
        return 'D';
    }
    else{
        return 'F';
    }
}

void calculate_performance(char grade){
    if (grade == 'A'){
        printf("Performance: *****\n");
    }
    else if (grade == 'B'){
        printf("Performance: ****\n");
    }
    else if (grade == 'C'){
        printf("Performance: ***\n");
    }
    else if (grade == 'D'){
        printf("Performance: **\n");
    }
}

void print_roll_numbers(int roll_numbers[],int roll_number_index,int total_students){
    if (roll_number_index == total_students){
        return;
    }

    printf("%d", roll_numbers[roll_number_index]);

    if (roll_number_index < total_students - 1){
        printf(" ");
    }

    print_roll_numbers(roll_numbers,roll_number_index + 1,total_students);
}

int main(void){
    int total_students;

    if (scanf("%d", &total_students) != 1){
        printf("Error: Invalid Input.\n");
        return 0;
    }

    if (total_students <= 0 ||total_students > MAX_STUDENTS){
        printf("Error: Invalid Input.\n");
        return 0;
    }

    int roll_numbers[MAX_STUDENTS];
    int totals[MAX_STUDENTS];
    float averages[MAX_STUDENTS];
    char grades[MAX_STUDENTS];
    struct Student students[MAX_STUDENTS];

    int student_count = 0;
    int input_character;

    while ((input_character = getchar()) != '\n' && input_character != EOF){
        if (!isspace((unsigned char)input_character)){
            printf("Error: Invalid Input.\n");
            return 0;
        }
    }

    for (int student_index = 0; student_index < total_students; student_index++){
        char line[MAX_LINE];
        char *tokens[MAX_TOKENS];
        int token_count = 0;

        struct Student student = {0};

        if (fgets(line, sizeof(line), stdin) == NULL){
            printf("Error: Invalid Input.\n");
            return 0;
        }

        if (strchr(line, '\n') == NULL && !feof(stdin)){
            while ((input_character = getchar()) != '\n' && input_character != EOF){
            }

            printf("Error: Invalid Input.\n");
            return 0;
        }

        char *current_token = strtok(line, " \t\r\n");

        while (current_token != NULL){
            if (token_count >= MAX_TOKENS){
                printf("Error: Invalid Input.\n");
                return 0;
            }

            tokens[token_count++] = current_token;
            current_token = strtok(NULL, " \t\r\n");
        }

        if (token_count < 5){
            printf("Error: Invalid Input.\n");
            return 0;
        }

        if (!parse_roll_number(tokens[0], &student.roll_no)){
            printf("Error: Invalid Input.\n");
            return 0;
        }

        int marks_start_index = token_count - 3;

        for (int mark_index = 0; mark_index < 3; mark_index++){
            if (!parse_mark(tokens[marks_start_index + mark_index],&student.marks[mark_index])){
                printf("Error: Invalid Input.\n");
                return 0;
            }
        }

        int total = calculate_total(student.marks[0],student.marks[1],student.marks[2]);

        if (total == -1){
            printf("Error: Invalid Input.\n");
            return 0;
        }

        float average = calculate_average(total);
        char grade = calculate_grade(average);

        size_t name_length = 0;

        for (int name_token_index = 1; name_token_index < marks_start_index; name_token_index++){
            if (!is_valid_name(tokens[name_token_index])){
                printf("Error: Invalid Input.\n");
                return 0;
            }

            size_t token_length = strlen(tokens[name_token_index]);
            size_t space_length = (name_token_index > 1) ? 1 : 0;

            if (name_length + space_length + token_length >= sizeof(student.name)){
                printf("Error: Invalid Input.\n");
                return 0;
            }

            if (space_length){
                student.name[name_length++] = ' ';
            }

            memcpy(student.name + name_length, tokens[name_token_index], token_length);
            name_length += token_length;
            student.name[name_length] = '\0';
        }

        if (name_length == 0){
            printf("Error: Invalid Input.\n");
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
            printf("\n");
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