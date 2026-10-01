#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validate(char expression[]){
    
    int expecting_number = 1;
    for(int index = 0; expression[index] != '\0'; index++){
        if(isalpha(expression[index])){
            printf("Error: Invalid expression.\n");
            return 0;
        }
        else if(isdigit(expression[index])){
            if(!expecting_number){
                printf("Error: Invalid expression.\n");
                return 0;
            }
            while(isdigit(expression[index])){
                index++;
            }
            index--;
            expecting_number = 0;
        }
        else if(expression[index] == '-' && expecting_number){

            index++;

            if(!isdigit(expression[index])){
                printf("Error: Invalid expression.\n");
                return 0;
            }

            while(isdigit(expression[index])){
                index++;
            }

            index--;
            expecting_number = 0;
        }
        else if(expression[index] == '+' || expression[index] == '-' || expression[index] == '*' || expression[index] == '/'){
            if(expecting_number){
                printf("Error: Invalid expression.\n");
                return 0;
            }
            expecting_number = 1;
        }
        else if((!(isdigit(expression[index]))) && (!(isspace(expression[index]))) && (expression[index] != '*' && expression[index] != '/' && expression[index] != '+' && expression[index] != '-')){
            printf("Error: Invalid expression.\n");
            return 0;
        }
          
    }
    
    if(expecting_number){
        printf("Error: Invalid expression.\n");
        return 0;
    }
    
    return 1;
}

int parse_operator(char expression[], char operator_array[]){

    int operator_count = 0;
    int expecting_number = 1;
    for(int index = 0; expression[index] != '\0'; index++){
        if(isdigit(expression[index])){
            while(isdigit(expression[index])){
                index++;
            }
            index--;
            expecting_number = 0;
        }
        else if(expression[index] == '-' && expecting_number){
            index++;
            while(isdigit(expression[index])){
                index++;
            }
            index--;
            expecting_number = 0;
        }
        else if(expression[index] == '+' || expression[index] == '-' || expression[index] == '*' || expression[index] == '/'){
            operator_array[operator_count] = expression[index];
            operator_count++;
            expecting_number = 1;
        }
    }
    return operator_count;
}
int parse_number(char expression[], long long number_array[]){

    long long number = 0;
    int has_number = 0;
    int number_count = 0;
    int expecting_number = 1;
    int is_negative = 0;
    for(int index = 0; expression[index] != '\0'; index++){
        if(expression[index] == '-' && expecting_number){
            is_negative = 1;
        }
        else if(isdigit(expression[index])){
            number = number * 10 + (expression[index] - '0');
            has_number = 1;
            expecting_number = 0;
        }
        else{
            if(has_number){
                if(is_negative){
                    number = -number;
                }
                number_array[number_count] = number;
                number = 0;
                is_negative = 0;
                has_number = 0;
                number_count++;
            }
            if(expression[index] == '+' || expression[index] == '-' ||
               expression[index] == '*' || expression[index] == '/'){
                expecting_number = 1;
            }
        }
    }
    if(has_number){
        if(is_negative){
            number = -number;
        }
        number_array[number_count] = number;
        number_count++;
    }
    return number_count;
}

long long evaluate_expression(long long number_array[], char operator_array[], int number_count, int operator_count, int *has_error){

    for(int operator_index = 0; operator_index < operator_count; operator_index++){

        if(operator_array[operator_index] == '*'){
            number_array[operator_index] = number_array[operator_index] * number_array[operator_index + 1];

            for(int number_index = operator_index + 1; number_index < number_count - 1; number_index++){
                number_array[number_index] = number_array[number_index + 1];
            }

            number_count--;

            for(int shift_index = operator_index; shift_index < operator_count - 1; shift_index++){
                operator_array[shift_index] = operator_array[shift_index + 1];
            }

            operator_count--;

            operator_index--;
        }

        else if(operator_array[operator_index] == '/'){
            if(number_array[operator_index + 1] != 0){
                number_array[operator_index] = number_array[operator_index] / number_array[operator_index + 1];
            }
            else{
                printf("Error: Division by zero.\n");
                *has_error = 1;
                return 0;
            }
            for(int number_index = operator_index + 1; number_index < number_count - 1; number_index++){
                number_array[number_index] = number_array[number_index + 1];
            }
            number_count--;
            for(int shift_index = operator_index; shift_index < operator_count - 1; shift_index++){
                operator_array[shift_index] = operator_array[shift_index + 1];
            }
            operator_count--;
            operator_index--;
        }
    }
    long long result = number_array[0];
    for(int index = 0; index < operator_count; index++){
        if(operator_array[index] == '+'){
            result = result + number_array[index + 1];
        }
        else if(operator_array[index] == '-'){
            result = result - number_array[index + 1];
        }
    }
    return result;
}

int main(){
    while(1){
        char expression[100];
        printf("Enter Expression (or type 'exit'): ");
        fgets(expression,100,stdin);
        if(strcmp(expression,"exit\n")==0){
            break;
        }
        int is_valid = validate(expression);
        if(!is_valid){
            continue;
        }
        char operator_array[50];
        long long number_array[50];
        int operator_count = parse_operator(expression,operator_array);
        int number_count = parse_number(expression,number_array);
        int has_error = 0;
        long long result = evaluate_expression(number_array,operator_array,number_count,operator_count,&has_error);
        if(!has_error){
            printf("%lld\n", result);
        }
        
    }
    return 0;

}