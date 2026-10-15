// Time Complexity: O(n)
// Space Complexity: O(1)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define STANDARD 150
#define PREMIUM 250
#define RECLINER 400

bool validate_name(char customer_name[]){
    if(customer_name[0] == '\0'){
        printf("Error: Name cannot be empty.\n");
        return false;
    }
    for(int character_index = 0; customer_name[character_index] != '\0'; character_index++){
        unsigned char current_character = (unsigned char)customer_name[character_index];
        if (!isalpha(current_character)){
            printf("Error: Name cannot contain special characters.\n");
            return false;
        }
    }
    return true;
}

float calculate_final_bill(float subtotal_amount, bool discount){
    if(discount){
        float discount_amount = subtotal_amount * 10.0f / 100.0f;
        return subtotal_amount - discount_amount;
    }
    return subtotal_amount;
}

bool calculate_discount(int number_of_tickets){
    return number_of_tickets >= 5 && number_of_tickets <= 20;
}

float calculate_subtotal(int category_choice, int number_of_tickets){
    int ticket_price = 0;

    switch (category_choice){
        case 1:
            ticket_price = STANDARD;
            break;
        case 2:
            ticket_price = PREMIUM;
            break;
        case 3:
            ticket_price = RECLINER;
            break;
        default:
            return -1.0f;
    }

    return (float)ticket_price * number_of_tickets;
}

void get_category_name(int category_choice,char category_name[]){
    switch(category_choice){
        case 1:
            strcpy(category_name,"Standard");
            return; 
        case 2:
            strcpy(category_name,"Premium");
            return;
        case 3:
            strcpy(category_name,"Recliner");
            return;
        default:
            return;
    }
}

int main(void){
    char customer_name[200];
    int category_choice = 0;
    int number_of_tickets=0;

    // Customer Name
    if (fgets(customer_name, sizeof(customer_name), stdin) == NULL) {
        printf("Error: Invalid Input.\n");
        return 0;
    }
    customer_name[strcspn(customer_name, "\n")] = '\0';
    if(!validate_name(customer_name)){
        return 0;
    }

    // Number of tickets
    if (scanf("%d", &number_of_tickets) != 1) {
        printf("Error: Invalid Ticket Count.\n");
        return 0;
    }
    if(number_of_tickets < 1 || number_of_tickets > 20){
        printf("Error: Invalid Ticket Count.\n");
        return 0;
    }

    // Category
    if(scanf("%d",&category_choice) != 1){
        printf("Error: Invalid Category.\n");
        return 0;
    }
    if(category_choice != 1 && category_choice != 2 && category_choice != 3){
        printf("Error: Invalid Category.\n");
        return 0;
    }
    
    char category_name[50];
    get_category_name(category_choice,category_name);
    float subtotal_amount = calculate_subtotal(category_choice,number_of_tickets);
    if(subtotal_amount < 0){
        printf("Error: Invalid Category.\n");
        return 0;
    }
    bool is_discount_applicable = calculate_discount(number_of_tickets);
    float final_bill_amount = calculate_final_bill(subtotal_amount,is_discount_applicable);
    float discount_amount = subtotal_amount - final_bill_amount;

    // Output
    printf("Customer: %s\n",customer_name);
    printf("Ticket Category: %s\n",category_name);
    printf("Tickets: %d\n",number_of_tickets);
    printf("Subtotal: %.2f\n",subtotal_amount);
    printf("Discount: %.2f\n",discount_amount);
    printf("Final Bill: %.2f\n",final_bill_amount);
    
    return 0;
}