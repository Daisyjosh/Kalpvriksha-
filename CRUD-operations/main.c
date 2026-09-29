#include <stdio.h>
#include <string.h>

struct User{
    int id;
    char name[50];
    int age;
};

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

    fprintf(file, "%d %s %d\n", u.id, u.name, u.age);
    fclose(file);
    return 1;
}

void read_users(){

    FILE *file;
    struct User u;

    file = fopen("users.txt", "r");

    if(file == NULL){
        printf("Error opening users.txt.\n");
        return;
    }

    while(fscanf(file, "%d %49s %d", &u.id, u.name, &u.age) == 3){
        printf("\n");
        printf("ID: %d\n", u.id);
        printf("Name: %s\n", u.name);
        printf("Age: %d\n", u.age);
        printf("\n");
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

    FILE *file;
    FILE *temp;

    struct User u;
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

    while(fscanf(file, "%d %49s %d", &u.id, u.name, &u.age) == 3){
        if(u.id == id){
            found = 1;
            printf("Enter New Name: ");
            scanf("%49s", u.name);
            printf("Enter New Age: ");
            scanf("%d", &u.age);
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
    }

    fclose(file);
    fclose(temp);

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

    struct User u;
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

    while(fscanf(file, "%d %49s %d", &u.id, u.name, &u.age) == 3){
        if(u.id == id){
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %d\n", u.id, u.name, u.age);
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

    struct User u;
    int largest_id = 0;
    file = fopen("users.txt", "r");

    if(file == NULL){
        return 1;
    }
    while(fscanf(file, "%d %49s %d", &u.id, u.name, &u.age) == 3){
        if(u.id > largest_id){
            largest_id = u.id;
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

        if(choice == 1){
            struct User u;

            u.id = get_next_id();
            printf("Enter Name: ");
            scanf("%49s", u.name);
            printf("Enter Age: ");
            scanf("%d", &u.age);
            if(create_user(u)){
                printf("\nUser created successfully.\n");
                printf("Your User ID is: %d\n", u.id);
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
            delete_user(id);
        }
        else if(choice == 5){
            return 0;
        }
        else{
            printf("Invalid choice.\n");
        }
    }
}