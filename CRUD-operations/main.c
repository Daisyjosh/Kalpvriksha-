#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct User{

    int id;
    char name[50];
    int age;
};

int validate_name(char name[]){

    int start = 0;
    int end = strlen(name) - 1;
    int name_index = 0;
    int is_valid_name = 1;
    int has_alphabet = 0;

    while(name[start] == ' '){
        start++;
    }

    while(end >= start && name[end] == ' '){
        end--;
    }

    if(start > end){
        return 0;
    }

    for(int index = start; index <= end; index++){
        name[name_index] = name[index];
        name_index++;
    }

    name[name_index] = '\0';
    for(int index = 0; name[index] != '\0'; index++){
        if(isalpha(name[index])){
            has_alphabet = 1;
        }
        else if(name[index] != ' '){
            is_valid_name = 0;
            break;
        }
    }
    if(!is_valid_name || !has_alphabet){
        return 0;
    }
    return 1;
}

int validate_age(int *age){

    char age_input[50];
    char extra;
    if(fgets(age_input, 50, stdin) == NULL){
        return 0;
    }
    if(sscanf(age_input, "%d %c", age, &extra) != 1){
        return 0;
    }
    if(*age <= 0){
        return 0;
    }
    return 1;
}

int create_file(){

    FILE *file;
    file = fopen("users.txt", "a");

    if(file == NULL){
        printf("Error opening users.txt.\n");
        return 0;
    }
    fclose(file);
    return 1;
}

int create_user(struct User u){

    FILE *file;

    file = fopen("users.txt", "a");
    if(file == NULL){
        printf("Error opening users.txt.\n");
        return 0;
    }

    fprintf(file, "%d %d %s\n", u.id, u.age, u.name);
    fclose(file);
    return 1;
}

void read_users(){

    FILE *file;
    struct User u;
    int has_users = 0;

    file = fopen("users.txt", "r");
    if(file == NULL){
        printf("Error opening file.\n");
        return;
    }
    while(fscanf(file, "%d %d %[^\n]", &u.id, &u.age, u.name) == 3){
        has_users = 1;
        printf("\n");
        printf("ID: %d\n", u.id);
        printf("Name: %s\n", u.name);
        printf("Age: %d\n", u.age);
        printf("\n");
    }
    if(!has_users){
        printf("No users found.\n");
    }
    fclose(file);
}

int replace_file(){

    if(rename("users.txt", "backup.txt") != 0){
        printf("Error creating backup file.\n");
        return 0;
    }
    if(rename("temp.txt", "users.txt") != 0){
        printf("Error replacing file.\n");
        if(rename("backup.txt", "users.txt") != 0){
            printf("Critical error: Could not restore original file.\n");
        }
        return 0;
    }
    if(remove("backup.txt") != 0){
        printf("Warning: Could not remove backup file.\n");
    }
    return 1;
}

void update_user(int id){

    int is_valid_update = 1;

    FILE *file;
    FILE *temp;
    struct User user;
    int found = 0;
    file = fopen("users.txt", "r");
    temp = fopen("temp.txt", "w");
    if(file == NULL || temp == NULL){
        printf("Error opening file.\n");
        if(file != NULL)
            fclose(file);
        if(temp != NULL)
            fclose(temp);
        return;
    }

    while(fscanf(file, "%d %d %[^\n]", &user.id, &user.age, user.name) == 3){
        if(user.id == id){
            found = 1;
            printf("Enter Name: ");
            fgets(user.name, 50, stdin);
            user.name[strcspn(user.name, "\n")] = '\0';

            if(!validate_name(user.name)){
                printf("Invalid input.\n");
                is_valid_update = 0;
                break;
            }

            printf("Enter Age: ");

            if(!validate_age(&user.age)){
                printf("Invalid input.\n");
                is_valid_update = 0;
                break;
            }
        }
        fprintf(temp, "%d %d %s\n", user.id, user.age, user.name);
    }

    fclose(file);
    fclose(temp);

    if(!is_valid_update){
        remove("temp.txt");
        return;
    }
    if(!found){
        printf("User with ID %d not found.\n", id);
        remove("temp.txt");
        return;
    }
    if(replace_file()){
        printf("User updated successfully.\n");
    }
}

void delete_user(int id){

    FILE *file;
    FILE *temp;

    struct User user;
    int found = 0;
    file = fopen("users.txt", "r");
    temp = fopen("temp.txt", "w");
    if(file == NULL || temp == NULL){
        printf("Error opening file.\n");
        if(file != NULL)
            fclose(file);
        if(temp != NULL)
            fclose(temp);
        return;
    }
    while(fscanf(file, "%d %d %[^\n]", &user.id, &user.age, user.name) == 3){
        if(user.id == id){
            found = 1;
            continue;
        }
        fprintf(temp, "%d %d %s\n", user.id, user.age, user.name);
    }
    fclose(file);
    fclose(temp);

    if(!found){
        printf("User with ID %d not found.\n", id);
        remove("temp.txt");
        return;
    }
    if(replace_file()){
        printf("User deleted successfully.\n");
    }
}

int get_next_id(){

    FILE *file;
    struct User user;
    int largest_id = 0;

    file = fopen("users.txt", "r");
    if(file == NULL){
        return 1;
    }

    while(fscanf(file, "%d %d %[^\n]", &user.id, &user.age, user.name) == 3){
        if(user.id > largest_id){
            largest_id = user.id;
        }
    }
    fclose(file);
    return largest_id + 1;
}

int main(){

    if(!create_file()){
        printf("Program cannot continue.\n");
        return 1;
    }

    while(1){
        printf("\n===== User Management =====\n");
        printf("1. Create User\n");
        printf("2. Display Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");

        int choice;
        printf("Enter Your Choice: ");

        if(scanf("%d", &choice) != 1){
            printf("Invalid input.\n");
            while(getchar() != '\n');
            continue;
        }

        if((choice != 1) && (choice != 2) && (choice != 3) && (choice != 4) && (choice != 5)){
            printf("Invalid input.\n");
            while(getchar() != '\n');
            continue;
        }

        while(getchar() != '\n');

        if(choice == 1){
            struct User user;
            user.id = get_next_id();
            printf("Enter Name: ");
            fgets(user.name, 50, stdin);
            user.name[strcspn(user.name, "\n")] = '\0';

            if(!validate_name(user.name)){
                printf("Invalid input.\n");
                continue;
            }

            printf("Enter Age: ");
            if(!validate_age(&user.age)){
                printf("Invalid input.\n");
                continue;
            }
            if(create_user(user)){
                printf("\nUser created successfully.\n");
                printf("Your User ID is: %d\n", user.id);
            }
            else{
                printf("User could not be created.\n");
            }
        }
        else if(choice == 2){
            read_users();
        }
        else if(choice == 3){
            int id;
            printf("Enter ID to update: ");

            if(scanf("%d", &id) != 1){
                printf("Invalid ID.\n");
                while(getchar() != '\n');
                continue;
            }
            if(id <= 0){
                printf("Invalid input.\n");
                while(getchar() != '\n');
                continue;
            }
            while(getchar() != '\n');
            update_user(id);
        }
        else if(choice == 4){

            int id;
            printf("Enter ID to delete: ");

            if(scanf("%d", &id) != 1){
                printf("Invalid ID.\n");
                while(getchar() != '\n');
                continue;
            }
            if(id <= 0){
                printf("Invalid input.\n");
                while(getchar() != '\n');
                continue;
            }
            delete_user(id);
        }
        else if(choice == 5){
            return 0;
        }
    }
}