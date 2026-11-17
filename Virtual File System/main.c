// Time Complexity: O(N + B + S)
// Space Complexity: O(B + E)

// N - Entries in directory
// B - Total disk blocks
// S - File content size
// E - Total filesystem entries

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define VFS_BLOCK_SIZE 512
#define VFS_BLOCK_COUNT 1024
#define VFS_MAX_NAME_LENGTH 50
#define VFS_MAX_COMMAND_LENGTH 4096

typedef struct FreeBlockNode {
    int block_index;
    struct FreeBlockNode *next;
    struct FreeBlockNode *prev;
} FreeBlockNode;

typedef struct VfsNode {
    char entry_name[VFS_MAX_NAME_LENGTH + 1];

    int is_directory;

    struct VfsNode *parent_directory;
    struct VfsNode *next;
    struct VfsNode *prev;
    struct VfsNode *children;

    int *block_indices;
    size_t allocated_block_count;
    size_t file_size;

} VfsNode;

unsigned char (*virtual_disk)[VFS_BLOCK_SIZE];

FreeBlockNode *free_block_nodes;
FreeBlockNode *free_list_head = NULL;
FreeBlockNode *free_list_tail = NULL;

size_t available_block_count = 0;

VfsNode *root_directory = NULL;
VfsNode *current_directory = NULL;

void trim_whitespace(char *text_buffer){

    size_t text_length = strlen(text_buffer);

    while (text_length > 0 &&
           isspace((unsigned char)text_buffer[text_length - 1])){
        text_buffer[--text_length] = '\0';
    }

    size_t start_index = 0;

    while (text_buffer[start_index] &&
           isspace((unsigned char)text_buffer[start_index])){
        start_index++;
    }

    if (start_index > 0){
        memmove(text_buffer, text_buffer + start_index, strlen(text_buffer + start_index) + 1);
    }
}

VfsNode *create_vfs_node(const char *entry_name, int is_directory){

    if (!entry_name || !*entry_name || strlen(entry_name) > VFS_MAX_NAME_LENGTH){
        return NULL;
    }

    VfsNode *node = calloc(1, sizeof(VfsNode));

    if (!node){
        return NULL;
    }

    strcpy(node->entry_name, entry_name);

    node->is_directory = is_directory;

    node->next = node;
    node->prev = node;

    return node;
}

VfsNode *find_child(VfsNode *directory_node, const char *entry_name){

    if (!directory_node || !directory_node->is_directory ||
        !directory_node->children){
        return NULL;
    }

    VfsNode *current_node = directory_node->children;

    do{

        if (strcmp(current_node->entry_name, entry_name) == 0){
            return current_node;
        }

        current_node = current_node->next;

    } while (current_node != directory_node->children);

    return NULL;
}

int insert_child(VfsNode *parent_directory, VfsNode *child_node){

    if (!parent_directory || !child_node || !parent_directory->is_directory){
        return 0;
    }

    if (find_child(parent_directory, child_node->entry_name)){
        return 0;
    }

    child_node->parent_directory = parent_directory;

    if (parent_directory->children == NULL){

        parent_directory->children = child_node;

        child_node->next = child_node;
        child_node->prev = child_node;

    }else{

        VfsNode *last_child = parent_directory->children->prev;

        last_child->next = child_node;
        child_node->prev = last_child;

        child_node->next = parent_directory->children;
        parent_directory->children->prev = child_node;
    }

    return 1;
}

void remove_child(VfsNode *child_node){

    if (!child_node || !child_node->parent_directory){
        return;
    }

    VfsNode *parent_directory = child_node->parent_directory;

    if (child_node->next == child_node){

        parent_directory->children = NULL;

    }else{

        child_node->prev->next = child_node->next;
        child_node->next->prev = child_node->prev;

        if (parent_directory->children == child_node){
            parent_directory->children = child_node->next;
        }
    }

    child_node->next = child_node;
    child_node->prev = child_node;
    child_node->parent_directory = NULL;
}

void initialize_free_list(){

    free_list_head = NULL;
    free_list_tail = NULL;
    available_block_count = 0;

    for (int block_position = 0; block_position < VFS_BLOCK_COUNT; block_position++){

        FreeBlockNode *free_block_node = &free_block_nodes[block_position];

        free_block_node->block_index = block_position;
        free_block_node->next = NULL;
        free_block_node->prev = free_list_tail;

        if (free_list_head == NULL){
            free_list_head = free_block_node;
        }
        else
            free_list_tail->next = free_block_node;

        free_list_tail = free_block_node;
        available_block_count++;
    }
}

int allocate_block(){

    if (free_list_head == NULL){
        return -1;
    }

    FreeBlockNode *node = free_list_head;

    int block_index = node->block_index;

    free_list_head = node->next;

    if (free_list_head){
        free_list_head->prev = NULL;
    }
    else
        free_list_tail = NULL;

    node->next = NULL;
    node->prev = NULL;

    available_block_count--;

    return block_index;
}

void release_block(int block_index){

    if (block_index < 0 || block_index >= VFS_BLOCK_COUNT){
        return;
    }

    FreeBlockNode *node = &free_block_nodes[block_index];

    memset(virtual_disk[block_index], 0, VFS_BLOCK_SIZE);

    node->block_index = block_index;
    node->next = NULL;
    node->prev = free_list_tail;

    if (free_list_head == NULL){
        free_list_head = node;
    }
    else
        free_list_tail->next = node;

    free_list_tail = node;

    available_block_count++;
}

int initialize_vfs(){

    virtual_disk = calloc(VFS_BLOCK_COUNT, sizeof(*virtual_disk));

    free_block_nodes = calloc(VFS_BLOCK_COUNT, sizeof(FreeBlockNode));

    if (!virtual_disk || !free_block_nodes){
        return 0;
    }

    initialize_free_list();

    root_directory = create_vfs_node("/", 1);

    if (!root_directory){
        return 0;
    }

    current_directory = root_directory;

    return 1;
}

void create_directory_command(const char *entry_name){

    if (!entry_name || !*entry_name){
        printf("Usage: mkdir directory_name\n");
        return;
    }

    if (find_child(current_directory, entry_name)){
        printf("Error: Entry already exists.\n");
        return;
    }

    VfsNode *directory_node = create_vfs_node(entry_name, 1);

    if (!directory_node){
        printf("Memory allocation failed.\n");
        return;
    }

    if (!insert_child(current_directory, directory_node)){
        free(directory_node);
        printf("Directory creation failed.\n");
        return;
    }

    printf("Directory '%s' created successfully.\n", entry_name);
}

void create_file_command(const char *entry_name){

    if (!entry_name || !*entry_name){
        printf("Usage: create filename\n");
        return;
    }

    if (find_child(current_directory, entry_name)){
        printf("Error: Entry already exists.\n");
        return;
    }

    VfsNode *file_node = create_vfs_node(entry_name, 0);

    if (!file_node){
        printf("Memory allocation failed.\n");
        return;
    }

    if (!insert_child(current_directory, file_node)){
        free(file_node);
        printf("File creation failed.\n");
        return;
    }

    printf("File '%s' created successfully.\n", entry_name);
}

void write_file_command(const char *entry_name, const char *file_content){

    VfsNode *file_node = find_child(current_directory, entry_name);

    if (!file_node){
        printf("Error: File not found.\n");
        return;
    }

    if (file_node->is_directory){
        printf("Error: Cannot write to a directory.\n");
        return;
    }

    size_t content_length = strlen(file_content);

    size_t required_block_count =
        (content_length + VFS_BLOCK_SIZE - 1) / VFS_BLOCK_SIZE;

    if (required_block_count > available_block_count + file_node->allocated_block_count){
        printf("Error: Not enough disk space.\n");
        return;
    }

    int *new_block_indices = NULL;

    if (required_block_count > 0){

        new_block_indices = malloc(required_block_count * sizeof(int));

        if (!new_block_indices){
            printf("Memory allocation failed.\n");
            return;
        }
    }

    for (size_t block_position = 0; block_position < file_node->allocated_block_count; block_position++){
        release_block(file_node->block_indices[block_position]);
    }

    free(file_node->block_indices);

    file_node->block_indices = NULL;
    file_node->allocated_block_count = 0;
    file_node->file_size = 0;

    for (size_t block_position = 0; block_position < required_block_count; block_position++){

        int block_index = allocate_block();

        if (block_index == -1){

            for (size_t allocated_position = 0; allocated_position < block_position; allocated_position++){
                release_block(new_block_indices[allocated_position]);
            }

            free(new_block_indices);

            printf("Error: Block allocation failed.\n");
            return;
        }

        new_block_indices[block_position] = block_index;
    }

    size_t content_offset = 0;

    for (size_t block_position = 0; block_position < required_block_count; block_position++){

        size_t remaining_bytes = content_length - content_offset;

        size_t bytes_to_copy =
            remaining_bytes < VFS_BLOCK_SIZE ? remaining_bytes : VFS_BLOCK_SIZE;

        memcpy(
            virtual_disk[new_block_indices[block_position]],
            file_content + content_offset,
            bytes_to_copy
        );

        content_offset += bytes_to_copy;
    }

    file_node->block_indices = new_block_indices;
    file_node->allocated_block_count = required_block_count;
    file_node->file_size = content_length;

    printf("Data written successfully (size=%zu bytes).\n", content_length);
}

void read_file_command(const char *entry_name){

    VfsNode *file_node = find_child(current_directory, entry_name);

    if (!file_node){
        printf("Error: File not found.\n");
        return;
    }

    if (file_node->is_directory){
        printf("Error: Cannot read a directory.\n");
        return;
    }

    if (file_node->file_size == 0){
        printf("File is empty.\n");
        return;
    }

    size_t remaining_bytes = file_node->file_size;

    for (size_t block_position = 0; block_position < file_node->allocated_block_count; block_position++){

        size_t bytes_to_read =
            remaining_bytes < VFS_BLOCK_SIZE ? remaining_bytes : VFS_BLOCK_SIZE;

        fwrite(
            virtual_disk[file_node->block_indices[block_position]],
            1,
            bytes_to_read,
            stdout
        );

        remaining_bytes -= bytes_to_read;
    }

    printf("\n");
}

void delete_file_command(const char *entry_name){

    VfsNode *file_node = find_child(current_directory, entry_name);

    if (!file_node){
        printf("Error: File not found.\n");
        return;
    }

    if (file_node->is_directory){
        printf("Error: Use rmdir for directories.\n");
        return;
    }

    // Return allocated blocks.
    for (size_t block_position = 0; block_position < file_node->allocated_block_count; block_position++){
        release_block(file_node->block_indices[block_position]);
    }

    free(file_node->block_indices);

    // Remove the file from the circular list.
    remove_child(file_node);

    free(file_node);

    printf("File deleted successfully.\n");
}

void remove_directory_command(const char *entry_name){

    VfsNode *directory_node = find_child(current_directory, entry_name);

    if (!directory_node){
        printf("Error: Directory not found.\n");
        return;
    }

    if (!directory_node->is_directory){
        printf("Error: Not a directory.\n");
        return;
    }

    if (directory_node->children){
        printf("Error: Directory is not empty.\n");
        return;
    }

    remove_child(directory_node);

    free(directory_node);

    printf("Directory removed successfully.\n");
}

void print_path_recursive(VfsNode *node){

    if (!node || node == root_directory){
        printf("/");
        return;
    }

    print_path_recursive(node->parent_directory);

    if (node->parent_directory != root_directory){
        printf("/");
    }

    printf("%s", node->entry_name);
}

void print_working_directory_command(){

    print_path_recursive(current_directory);
    printf("\n");
}

VfsNode *resolve_directory(const char *path){

    if (!path || !*path){
        return current_directory;
    }

    if (strlen(path) >= VFS_MAX_COMMAND_LENGTH){
        return NULL;
    }

    char path_buffer[VFS_MAX_COMMAND_LENGTH];

    strcpy(path_buffer, path);

    VfsNode *current_node =
        (path_buffer[0] == '/') ? root_directory : current_directory;

    char *path_component = strtok(path_buffer, "/");

    while (path_component){

        if (strcmp(path_component, ".") == 0){
            /* Current directory: no change required. */
        }else if (strcmp(path_component, "..") == 0){

            if (current_node->parent_directory){
                current_node = current_node->parent_directory;
            }

        }else{

            VfsNode *next_directory = find_child(current_node, path_component);

            if (!next_directory || !next_directory->is_directory){
                return NULL;
            }

            current_node = next_directory;
        }

        path_component = strtok(NULL, "/");
    }

    return current_node;
}

void change_directory_command(const char *path){

    if (!path || !*path){
        printf("Usage: cd directory_name\n");
        return;
    }

    VfsNode *target_directory = resolve_directory(path);

    if (!target_directory){
        printf("Error: Directory not found.\n");
        return;
    }

    current_directory = target_directory;

    printf("Moved to ");
    print_working_directory_command();
}

void list_directory_command(const char *path){

    VfsNode *directory_node =
        (path && *path) ? resolve_directory(path) : current_directory;

    if (!directory_node){
        printf("Error: Directory not found.\n");
        return;
    }

    if (!directory_node->children){
        printf("(empty)\n");
        return;
    }

    VfsNode *current_node = directory_node->children;

    do{
        if (current_node->is_directory){
            printf("%s/\n", current_node->entry_name);
        }else{
            printf("%s\n", current_node->entry_name);
        }

        current_node = current_node->next;

    } while (current_node != directory_node->children);
}

void display_disk_usage_command(){

    size_t used_block_count = VFS_BLOCK_COUNT - available_block_count;

    double disk_usage_percentage =
        ((double)used_block_count / VFS_BLOCK_COUNT) * 100.0;

    printf("Total Blocks: %d\n", VFS_BLOCK_COUNT);
    printf("Used Blocks: %zu\n", used_block_count);
    printf("Free Blocks: %zu\n", available_block_count);
    printf("Disk Usage: %.2f%%\n", disk_usage_percentage);
}

void cleanup_directory(VfsNode *directory_node){

    if (!directory_node || !directory_node->children){
        return;
    }

    VfsNode *first_child = directory_node->children;
    VfsNode *current_node = first_child;

    do{

        VfsNode *next_child = current_node->next;

        if (current_node->is_directory){
            cleanup_directory(current_node);
        }

        free(current_node->block_indices);
        free(current_node);

        current_node = next_child;

    } while (current_node != first_child);

    directory_node->children = NULL;
}

void cleanup_vfs(){

    if (root_directory){

        cleanup_directory(root_directory);

        free(root_directory);

        root_directory = NULL;
        current_directory = NULL;
    }

    free(free_block_nodes);
    free_block_nodes = NULL;

    free(virtual_disk);
    virtual_disk = NULL;

    free_list_head = NULL;
    free_list_tail = NULL;
    available_block_count = 0;
}

void execute_command(char *command_line){

    trim_whitespace(command_line);

    if (!*command_line){
        return;
    }

    char *command_name = command_line;
    char *argument_cursor = command_name;

    while (*argument_cursor && !isspace((unsigned char)*argument_cursor)){
        argument_cursor++;
    }

    if (*argument_cursor){
        *argument_cursor = '\0';
        argument_cursor++;
    }

    while (*argument_cursor && isspace((unsigned char)*argument_cursor)){
        argument_cursor++;
    }

    char *command_argument = argument_cursor;

    while (*argument_cursor && !isspace((unsigned char)*argument_cursor)){
        argument_cursor++;
    }

    char *additional_argument = NULL;

    if (*argument_cursor){

        *argument_cursor = '\0';
        additional_argument = argument_cursor + 1;

        while (*additional_argument && isspace((unsigned char)*additional_argument)){
            additional_argument++;
        }
    }
    if (strcmp(command_name, "mkdir") == 0){

        create_directory_command(command_argument);

    }else if (strcmp(command_name, "create") == 0){

        create_file_command(command_argument);

    }else if (strcmp(command_name, "write") == 0){

        if (!*command_argument || !additional_argument || !*additional_argument){
            printf("Usage: write filename \"content\"\n");
            return;
        }

        trim_whitespace(additional_argument);

        size_t text_length = strlen(additional_argument);

        if (text_length >= 2 &&
            additional_argument[0] == '"' &&
            additional_argument[text_length - 1] == '"'){

            additional_argument[text_length - 1] = '\0';
            additional_argument++;
        }

        write_file_command(command_argument, additional_argument);

    }else if (strcmp(command_name, "read") == 0){

        read_file_command(command_argument);

    }else if (strcmp(command_name, "delete") == 0){

        delete_file_command(command_argument);

    }else if (strcmp(command_name, "rmdir") == 0){

        remove_directory_command(command_argument);

    }else if (strcmp(command_name, "ls") == 0){

        list_directory_command(command_argument);

    }else if (strcmp(command_name, "cd") == 0){

        change_directory_command(command_argument);

    }else if (strcmp(command_name, "pwd") == 0){

        print_working_directory_command();

    }else if (strcmp(command_name, "df") == 0){

        display_disk_usage_command();

    }else if (strcmp(command_name, "exit") == 0){

        cleanup_vfs();

        printf("Memory released. Exiting program...\n");

        exit(0);

    }else{

        printf("Unknown command.\n");
    }
}

int main(){

    if (!initialize_vfs()){

        printf("VFS initialization failed.\n");

        cleanup_vfs();

        return 1;
    }

    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char input_buffer[VFS_MAX_COMMAND_LENGTH];

    while (1){

        print_path_recursive(current_directory);
        printf(" > ");

        if (!fgets(input_buffer, sizeof(input_buffer), stdin)){
            printf("\n");
            break;
        }

        if (!strchr(input_buffer, '\n') && !feof(stdin)){

            int input_character;

            while ((input_character = getchar()) != '\n' && input_character != EOF);

            printf("Error: Command too long.\n");
            continue;
        }

        execute_command(input_buffer);
    }

    cleanup_vfs();

    return 0;
}
