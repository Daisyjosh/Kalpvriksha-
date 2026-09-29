#include <stdio.h>
#include <string.h>
#include <ctype.h>

int validate(char arr[]){
    
    int expectingNumber = 1;
    for(int i = 0; arr[i] != '\0'; i++){
        if(isalpha(arr[i])){
            printf("Error: Invalid expression.\n");
            return 0;
        }
        else if(isdigit(arr[i])){
            if(!expectingNumber){
                printf("Error: Invalid expression.\n");
                return 0;
            }
            while(isdigit(arr[i])){
                i++;
            }
            i--;
            expectingNumber = 0;
        }
        else if(arr[i] == '+' || arr[i] == '-' || arr[i] == '*' || arr[i] == '/'){
            if(expectingNumber){
                printf("Error: Invalid expression.\n");
                return 0;
            }
            expectingNumber = 1;
        }
        else if((!(isdigit(arr[i]))) && (!(isspace(arr[i]))) && (arr[i] != '*' && arr[i] != '/' && arr[i] != '+' && arr[i] != '-')){
            printf("Error: Invalid expression.\n");
            return 0;
        }
        
    }
    
    if(expectingNumber){
        printf("Error: Invalid expression.\n");
        return 0;
    }
    
    return 1;
}

int parse_operator(char arr[],char OpArr[]){
    int count = 0;
    for(int i=0; arr[i] != '\0'; i++){
        if(arr[i] == '+' || arr[i] == '-' || arr[i] == '*' || arr[i] == '/'){
            OpArr[count] = arr[i];
            count++;
        }
    }
    // printf("Operators: \n");
    // for(int i=0; i<count; i++){
    //     printf("%c ",OpArr[i]);
    // }
    // printf("\n");
    return count;
}  

int parse_number(char arr[],int numArr[]){
    int num = 0;
    int flag = 0;
    int count = 0;
    for(int i=0; arr[i] != '\0'; i++){
        if(isdigit(arr[i])){
            num = num * 10 + (arr[i]-'0');
            flag = 1;
        }
        else{
            if(flag){
                numArr[count] = num;
                num = 0;
                flag = 0;
                count++;
            }
        }
    }
    if(flag){
        numArr[count] = num;
        count++;
    }
    
    // printf("Numbers:\n");
    // for(int i = 0; i < count; i++){
    //     printf("%d ",numArr[i]);
    // }
    // printf("\n");
    return count;
}

int evaluate_expression(int numArr[], char OpArr[], int number_count, int operator_count,int *error){
    
    // first preference : "*" & "/"
    
    for(int i = 0; i < operator_count; i++){
        if(OpArr[i] == '*'){
            numArr[i] = numArr[i] * numArr[i+1];
            
            // shift remaining numbers left
            for(int j = i+1; j < number_count-1; j++){
                numArr[j] = numArr[j+1];
            }
            number_count--;
            
            // shift remaining operators left
            for(int j = i; j < operator_count-1; j++){
                OpArr[j] = OpArr[j+1];
            }
            
            operator_count--;
            
            i--;
        }
        else if (OpArr[i] == '/') {
            if(numArr[i+1] != 0){
                numArr[i] = numArr[i] / numArr[i + 1];
            }
            else{
                printf("Error: Division by zero.\n");
                *error = 1;
                return 0;
            }
            // Shift numbers
            for (int j = i + 1; j < number_count - 1; j++) {
                numArr[j] = numArr[j + 1];
            }

            number_count--;

            // Shift operators
            for (int j = i; j < operator_count - 1; j++) {
                OpArr[j] = OpArr[j + 1];
            }

            operator_count--;

            i--;
        }
        
    }
    
    // second preference: "+" & "-"
    
    int result = numArr[0];
    for(int i = 0; i < operator_count; i++){
        if (OpArr[i] == '+') {

            result = result + numArr[i + 1];
        }

        else if (OpArr[i] == '-') {

            result = result - numArr[i + 1];
        }
    }
    
    return result;
}
int main(){
    char arr[100];
    fgets(arr,100,stdin);
    int valid = validate(arr);
    if(!valid){
        return 0;
    }
    char OpArr[50];
    int numArr[50];
    int operator_count = parse_operator(arr,OpArr);
    int number_count = parse_number(arr,numArr);
    int error = 0;
    int result = evaluate_expression(numArr,OpArr,number_count,operator_count,&error);
    if(!error){
        printf("%d\n",result);
    }
    else{
        return 0;
    }
    return 0;
}