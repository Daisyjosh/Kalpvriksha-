// Time Complexity - O(N)
// Space Complexity - O(N)

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define MAX_NAME_SIZE 200
#define MAX_DESIGNATION_SIZE 50
#define DESIGNATION_COUNT 7

enum Designation{
    CEO = 1,
    HR_MANAGER,
    ENGINEERING_MANAGER,
    SENIOR_SOFTWARE_ENGINEER,
    SOFTWARE_ENGINEER,
    QA_ENGINEER,
    HR_EXECUTIVE
};

const char *designations[DESIGNATION_COUNT] = {
    "CEO",
    "HR Manager",
    "Engineering Manager",
    "Senior Software Engineer",
    "Software Engineer",
    "QA Engineer",
    "HR Executive"
};

struct Employee;

struct ReportNode{
    struct Employee *employee;
    struct ReportNode *next;
};

struct Employee{
    int employee_id;
    char name[MAX_NAME_SIZE];
    char designation[MAX_DESIGNATION_SIZE];
    struct Employee *manager;
    struct ReportNode *reports;
    struct Employee *next;
};

struct Employee *employees = NULL;

int read_line(char buffer[], size_t size){
    if(fgets(buffer, (int)size, stdin) == NULL){
        return 0;
    }

    size_t length = strlen(buffer);

    if(length > 0 && buffer[length - 1] == '\n'){
        buffer[length - 1] = '\0';
        return 1;
    }

    if (feof(stdin)){
        return 1;
    }

    int character;
    while ((character = getchar()) != '\n' && character != EOF);

    return -1;
}

int read_integer(const char *prompt, int *value){
    char input[100];
    char *endptr;
    long parsed_value;
    int result;

    printf("%s", prompt);
    result = read_line(input, sizeof(input));

    if(result == 0){
        printf("Error: No input received.\n");
        return 0;
    }
    if (result == -1){
        printf("Error: Input is too long.\n");
        return 0;
    }

    errno = 0;
    parsed_value = strtol(input, &endptr, 10);

    if(endptr == input){
        printf("Error: Invalid input. Enter an integer.\n");
        return 0;
    }

    while(isspace((unsigned char)*endptr)){
        endptr++;
    }

    if(errno == ERANGE || *endptr != '\0' || parsed_value < INT_MIN || parsed_value > INT_MAX){
        printf("Error: Invalid integer input.\n");
        return 0;
    }

    *value = (int)parsed_value;
    return 1;
}

int isvalid_name(char name[]){
    size_t length = strlen(name);
    size_t start = 0;
    size_t end = length;
    size_t output_index = 0;
    int has_letter = 0;

    while(start < end && isspace((unsigned char)name[start])){
        start++;
    }

    while (end > start && isspace((unsigned char)name[end - 1])){
        end--;
    }

    for (size_t i = start; i < end; i++){
        unsigned char ch = (unsigned char)name[i];

        if (isalpha(ch)) {
            has_letter = 1;
            name[output_index++] = (char)ch;
        } else if (isspace(ch)) {
            name[output_index++] = ' ';
        } else {
            return 0;
        }
    }

    name[output_index] = '\0';
    return has_letter;
}

int get_designation_rank(const char designation[]){
    if(strcmp(designation, "CEO") == 0) {
        return 1;
    }

    if(strcmp(designation, "Engineering Manager") == 0 || strcmp(designation, "HR Manager") == 0){
        return 2;
    }

    if(strcmp(designation, "Senior Software Engineer") == 0 || strcmp(designation, "HR Executive") == 0){
        return 3;
    }

    if(strcmp(designation, "Software Engineer") == 0 || strcmp(designation, "QA Engineer") == 0){
        return 4;
    }

    return -1;
}

struct Employee *find_employee(int id){
    struct Employee *current = employees;

    while (current != NULL) {
        if (current->employee_id == id){
            return current;
        }
        current = current->next;
    }

    return NULL;
}

int add_report(struct Employee *manager, struct Employee *employee){
    struct ReportNode *new_node = malloc(sizeof(*new_node));

    if(new_node == NULL){
        return 0;
    }

    new_node->employee = employee;
    new_node->next = manager->reports;
    manager->reports = new_node;
    return 1;
}

void remove_report_link(struct Employee *manager,struct Employee *employee){
    if(manager == NULL) {
        return;
    }

    struct ReportNode *current = manager->reports;
    struct ReportNode *previous = NULL;

    while(current != NULL){
        if(current->employee == employee){
            if (previous == NULL) {
                manager->reports = current->next;
            }
            else{
                previous->next = current->next;
            }

            free(current);
            return;
        }

        previous = current;
        current = current->next;
    }
}

int count_direct_reports(struct Employee *employee){
    int count = 0;
    struct ReportNode *current = employee->reports;

    while(current != NULL){
        count++;
        current = current->next;
    }

    return count;
}

struct Employee *find_sibling_with_fewest_reports(struct Employee *employee){
    if(employee->manager == NULL){
        return NULL;
    }

    struct Employee *sibling = NULL;
    int minimum_reports = INT_MAX;
    struct ReportNode *current = employee->manager->reports;

    while(current != NULL){
        struct Employee *candidate = current->employee;

        if(candidate != employee){
            int report_count = count_direct_reports(candidate);

            if(sibling == NULL || report_count < minimum_reports){
                sibling = candidate;
                minimum_reports = report_count;
            }
        }

        current = current->next;
    }

    return sibling;
}

void add_employee(void){
    printf("\n===== Add Employee =====\n");

    struct Employee *employee = malloc(sizeof(*employee));

    if(employee == NULL){
        printf("Error: Memory allocation failed.\n");
        return;
    }

    employee->manager = NULL;
    employee->reports = NULL;
    employee->next = NULL;

    if(!read_integer("Enter Employee ID: ", &employee->employee_id)){
        free(employee);
        return;
    }

    if(employee->employee_id <= 0){
        printf("Error: Employee ID must be positive.\n");
        free(employee);
        return;
    }

    if(find_employee(employee->employee_id) != NULL){
        printf("Error: Employee ID already exists.\n");
        free(employee);
        return;
    }

    char name_input[MAX_NAME_SIZE];
    printf("Enter Name: ");
    int line_result = read_line(name_input, sizeof(name_input));

    if(line_result == 0){
        printf("Error: No name received.\n");
        free(employee);
        return;
    }

    if(line_result == -1){
        printf("Error: Name is too long.\n");
        free(employee);
        return;
    }

    if(!isvalid_name(name_input)){
        printf("Error: Invalid name. Use letters and spaces only.\n");
        free(employee);
        return;
    }

    strcpy(employee->name, name_input);

    printf("\n===== DESIGNATION =====\n");
    for(int index = 0; index < DESIGNATION_COUNT; index++){
        printf("%d. %s\n", index + 1, designations[index]);
    }

    int designation_choice;
    if(!read_integer("Enter designation choice: ", &designation_choice)){
        free(employee);
        return;
    }

    if(designation_choice < 1 || designation_choice > DESIGNATION_COUNT){
        printf("Error: Invalid designation choice.\n");
        free(employee);
        return;
    }

    strcpy(employee->designation, designations[designation_choice - 1]);

    int manager_id;
    if(!read_integer("Enter Reporting Manager ID (0 if none): ",&manager_id)){
        free(employee);
        return;
    }

    if(manager_id < 0){
        printf("Error: Manager ID cannot be negative.\n");
        free(employee);
        return;
    }

    if(designation_choice == CEO && manager_id != 0){
        printf("Error: CEO cannot have a reporting manager.\n");
        free(employee);
        return;
    }
    if(designation_choice != CEO && manager_id == 0){
        printf("Error: Every non-CEO employee must have a reporting manager.\n");
        free(employee);
        return;
    }

    struct Employee *manager = NULL;

    if(manager_id != 0){
        if(manager_id == employee->employee_id){
            printf("Error: An employee cannot report to themselves.\n");
            free(employee);
            return;
        }

        manager = find_employee(manager_id);

        if(manager == NULL){
            printf("Error: Reporting manager not found.\n");
            free(employee);
            return;
        }

        int employee_rank = get_designation_rank(employee->designation);
        int manager_rank = get_designation_rank(manager->designation);

        if(employee_rank == -1 || manager_rank == -1 ||manager_rank >= employee_rank){
            printf("Error: Reporting manager must be senior to the employee.\n");
            free(employee);
            return;
        }

        if(!add_report(manager, employee)){
            printf("Error: Memory allocation failed.\n");
            free(employee);
            return;
        }

        employee->manager = manager;
    }

    employee->next = employees;
    employees = employee;

    printf("\nEmployee added successfully!\n");
    printf("Employee ID: %d\n", employee->employee_id);
    printf("Name: %s\n", employee->name);
    printf("Designation: %s\n", employee->designation);

    if(employee->manager != NULL){
        printf("Reporting Manager: %s (ID: %d)\n",
               employee->manager->name,
               employee->manager->employee_id);
    } 
    else{
        printf("Reporting Manager: None (Top-level employee)\n");
    }
}

void remove_employee(void){
    printf("\n===== Remove Employee =====\n");

    int employee_id;
    if(!read_integer("Enter Employee ID to remove: ", &employee_id)){
        return;
    }

    if(employee_id <= 0){
        printf("Error: Employee ID must be positive.\n");
        return;
    }

    struct Employee *employee = find_employee(employee_id);

    if(employee == NULL){
        printf("Error: Employee not found.\n");
        return;
    }

    if(strcmp(employee->designation, "CEO") == 0){
        printf("Error: CEO cannot be removed.\n");
        return;
    }

    struct Employee *manager = employee->manager;
    struct Employee *sibling = find_sibling_with_fewest_reports(employee);
    struct Employee *transfer_manager = NULL;

    if(employee->reports != NULL){
        if(sibling != NULL){
            transfer_manager = sibling;
        } 
        else if (manager != NULL){
            transfer_manager = manager;
        }

        if(transfer_manager != NULL){
            struct ReportNode *current = employee->reports;
            while(current != NULL){
                current->employee->manager = transfer_manager;
                current = current->next;
            }

            struct ReportNode *last = employee->reports;
            while(last->next != NULL){
                last = last->next;
            }

            last->next = transfer_manager->reports;
            transfer_manager->reports = employee->reports;
            employee->reports = NULL;
        } 
        else{
            struct ReportNode *current = employee->reports;

            while(current != NULL){
                current->employee->manager = NULL;

                struct ReportNode *next_node = current->next;
                free(current);
                current = next_node;
            }

            employee->reports = NULL;
        }
    }

    remove_report_link(manager, employee);

    if(employees == employee){
        employees = employee->next;
    } 
    else{
        struct Employee *previous = employees;

        while(previous != NULL && previous->next != employee){
            previous = previous->next;
        }

        if(previous != NULL){
            previous->next = employee->next;
        }
    }

    free(employee);
    printf("Employee removed successfully.\n");
}

void display_hierarchy_recursive(struct Employee *employee, int level){
    for (int i = 0; i < level; i++){
        printf("    ");
    }

    printf("%d - %s - %s\n",employee->employee_id,employee->name,employee->designation);

    struct ReportNode *current = employee->reports;

    while(current != NULL){
        display_hierarchy_recursive(current->employee, level + 1);
        current = current->next;
    }
}

int count_employees_recursive(struct Employee *employee){
    if(employee == NULL){
        return 0;
    }

    int count = 1;
    struct ReportNode *current = employee->reports;

    while(current != NULL){
        count += count_employees_recursive(current->employee);
        current = current->next;
    }

    return count;
}

void display_employee_hierarchy(void){
    printf("\n===== Display Employee Hierarchy =====\n");

    int employee_id;
    if(!read_integer("Enter Employee ID: ", &employee_id)){
        return;
    }

    if(employee_id <= 0){
        printf("Error: Employee ID must be positive.\n");
        return;
    }

    struct Employee *employee = find_employee(employee_id);

    if(employee == NULL){
        printf("Error: Employee not found.\n");
        return;
    }

    printf("\n===== EMPLOYEE HIERARCHY =====\n");
    display_hierarchy_recursive(employee, 0);

    int total_under = count_employees_recursive(employee) - 1;
    printf("Total Employees Under %d: %d\n", employee->employee_id,total_under);
}

void display_complete_hierarchy(void){
    printf("\n===== ORGANIZATION HIERARCHY =====\n");

    if (employees == NULL) {
        printf("No employees in the organization.\n");
        printf("Total Employees: 0\n");
        return;
    }

    int total_employees = 0;
    struct Employee *current = employees;

    while(current != NULL){
        if(current->manager == NULL){
            display_hierarchy_recursive(current, 0);
            total_employees += count_employees_recursive(current);
        }

        current = current->next;
    }

    printf("Total Employees: %d\n",total_employees);
}

void free_all_employees(void){
    struct Employee *current = employees;

    while(current != NULL){
        struct Employee *next_employee = current->next;
        struct ReportNode *report = current->reports;

        while(report != NULL){
            struct ReportNode *next_report = report->next;
            free(report);
            report = next_report;
        }

        free(current);
        current = next_employee;
    }

    employees = NULL;
}

int main(void){
    while (true){
        printf("\n===== EMPLOYEE HIERARCHY MANAGEMENT =====\n");
        printf("1. Add Employee\n");
        printf("2. Remove Employee\n");
        printf("3. Display Employee Hierarchy\n");
        printf("4. Display Complete Hierarchy\n");
        printf("5. Exit\n");

        int choice;
        if(!read_integer("Enter your choice: ", &choice)){
            if(feof(stdin)){
                free_all_employees();
                return 0;
            }
            continue;
        }

        switch(choice){
            case 1:
                add_employee();
                break;
            case 2:
                remove_employee();
                break;
            case 3:
                display_employee_hierarchy();
                break;
            case 4:
                display_complete_hierarchy();
                break;
            case 5:
                free_all_employees();
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Error: Invalid choice. Enter a number from 1 to 5.\n");
        }
    }
}
