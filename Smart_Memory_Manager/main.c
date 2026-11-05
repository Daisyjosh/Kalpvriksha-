
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_POOL_SIZE 1000000
#define FIRST_FIT 1
#define BEST_FIT 2
#define WORST_FIT 3

static int input_eof=0;

typedef struct{
    size_t start;
    size_t size;
    uint64_t allocation_id;
    int is_allocated;
} Block;

typedef struct{
    unsigned char *pool;
    Block *blocks;
    size_t pool_size;
    size_t block_count;
    size_t capacity;
    uint64_t next_id;
    uint64_t last_failed_request;
    int strategy;
} MemoryManager;

int read_unsigned(const char *message,uint64_t *value){
    char input[128];
    char *end;

    printf("%s",message);

    if(fgets(input,sizeof(input),stdin)==NULL){
        input_eof=1;
        return 0;
    }

    if(strchr(input,'\n')==NULL&&!feof(stdin)){
        int character;

        while((character=getchar())!='\n'&&character!=EOF);

        return 0;
    }

    errno=0;

    char *start=input;

    while(isspace((unsigned char)*start)){
        start++;
    }

    if(*start=='-'||*start=='\0'){
        return 0;
    }

    unsigned long long number=strtoull(start,&end,10);

    if(errno==ERANGE||start==end){
        return 0;
    }

    while(isspace((unsigned char)*end)){
        end++;
    }

    if(*end!='\0'){
        return 0;
    }

    *value=(uint64_t)number;

    return 1;
}

int initialize_pool(MemoryManager *manager,size_t pool_size){
    manager->pool=malloc(pool_size*sizeof(unsigned char));

    if(manager->pool==NULL){
        printf("Memory allocation failed.\n");
        return 0;
    }

    manager->blocks=malloc(sizeof(Block));

    if(manager->blocks==NULL){
        printf("Memory allocation failed.\n");
        free(manager->pool);
        manager->pool=NULL;
        return 0;
    }

    manager->pool_size=pool_size;
    manager->block_count=1;
    manager->capacity=1;
    manager->next_id=1;
    manager->last_failed_request=0;

    memset(manager->pool,0,pool_size);

    manager->blocks[0].start=0;
    manager->blocks[0].size=pool_size;
    manager->blocks[0].allocation_id=0;
    manager->blocks[0].is_allocated=0;

    return 1;
}

int grow_metadata(MemoryManager *manager){
    if(manager->capacity>SIZE_MAX/2){
        return 0;
    }

    size_t new_capacity=manager->capacity*2;

    if(new_capacity>SIZE_MAX/sizeof(Block)){
        return 0;
    }

    Block *temp=realloc(manager->blocks,new_capacity*sizeof(Block));

    if(temp==NULL){
        return 0;
    }

    manager->blocks=temp;
    manager->capacity=new_capacity;

    return 1;
}

int ensure_capacity(MemoryManager *manager){
    if(manager->block_count<manager->capacity){
        return 1;
    }

    return grow_metadata(manager);
}

int first_fit(MemoryManager *manager,size_t requested_size){
    for(size_t i=0;i<manager->block_count;i++){
        if(!manager->blocks[i].is_allocated&&manager->blocks[i].size>=requested_size){
            return (int)i;
        }
    }

    return -1;
}

int best_fit(MemoryManager *manager,size_t requested_size){
    int selected=-1;
    size_t smallest=SIZE_MAX;

    for(size_t i=0;i<manager->block_count;i++){
        if(!manager->blocks[i].is_allocated&&manager->blocks[i].size>=requested_size){
            if(manager->blocks[i].size<smallest){
                smallest=manager->blocks[i].size;
                selected=(int)i;
            }
        }
    }

    return selected;
}

int worst_fit(MemoryManager *manager,size_t requested_size){
    int selected=-1;
    size_t largest=0;

    for(size_t i=0;i<manager->block_count;i++){
        if(!manager->blocks[i].is_allocated&&manager->blocks[i].size>=requested_size){
            if(manager->blocks[i].size>largest){
                largest=manager->blocks[i].size;
                selected=(int)i;
            }
        }
    }

    return selected;
}

int find_suitable_block(MemoryManager *manager,size_t requested_size){
    switch(manager->strategy){
        case FIRST_FIT:
        {
            return first_fit(manager,requested_size);
        }

        case BEST_FIT:
        {
            return best_fit(manager,requested_size);
        }

        case WORST_FIT:
        {
            return worst_fit(manager,requested_size);
        }

        default:
        {
            return -1;
        }
    }
}

int find_allocation(MemoryManager *manager,uint64_t allocation_id){
    for(size_t i=0;i<manager->block_count;i++){
        if(manager->blocks[i].is_allocated&&manager->blocks[i].allocation_id==allocation_id){
            return (int)i;
        }
    }

    return -1;
}

void calculate_memory(MemoryManager *manager,size_t *allocated,size_t *allocated_blocks,size_t *free_memory,size_t *free_blocks,size_t *largest_free){
    *allocated=0;
    *free_memory=0;
    *free_blocks=0;
    *largest_free=0;
    *allocated_blocks=0;

    for(size_t i=0;i<manager->block_count;i++){
        Block *block=&manager->blocks[i];

        if(block->is_allocated){
            *allocated+=block->size;
            (*allocated_blocks)++;
        }
        else{
            *free_memory+=block->size;
            (*free_blocks)++;

            if(block->size>*largest_free){
                *largest_free=block->size;
            }
        }
    }
}

int allocate_memory(MemoryManager *manager,size_t requested_size){
    if(requested_size==0||requested_size>manager->pool_size){
        printf("Invalid allocation size.\n");
        return 0;
    }

    if(manager->next_id==0||manager->next_id==UINT64_MAX){
        printf("Allocation ID limit reached.\n");
        return 0;
    }

    int index=find_suitable_block(manager,requested_size);

    if(index==-1){
        printf("Allocation failed: no contiguous free block is large enough.\n");

        manager->last_failed_request=requested_size;

        return 0;
    }

    size_t original_start=manager->blocks[index].start;
    size_t original_size=manager->blocks[index].size;

    if(original_size>requested_size){
        if(!ensure_capacity(manager)){
            printf("Allocation failed.\n");
            return 0;
        }
    }

    uint64_t allocation_id=manager->next_id++;

    if(original_size==requested_size){
        manager->blocks[index].is_allocated=1;
        manager->blocks[index].allocation_id=allocation_id;
    }
    else{
        for(size_t i=manager->block_count;i>(size_t)index+1;i--){
            manager->blocks[i]=manager->blocks[i-1];
        }

        manager->blocks[index].start=original_start;
        manager->blocks[index].size=requested_size;
        manager->blocks[index].allocation_id=allocation_id;
        manager->blocks[index].is_allocated=1;

        manager->blocks[index+1].start=original_start+requested_size;
        manager->blocks[index+1].size=original_size-requested_size;
        manager->blocks[index+1].allocation_id=0;
        manager->blocks[index+1].is_allocated=0;

        manager->block_count++;
    }

    manager->last_failed_request=0;

    printf("Allocation successful. ID: %" PRIu64 ", Start: %zu, Size: %zu\n",allocation_id,original_start,requested_size);

    return 1;
}

void merge_free_blocks(MemoryManager *manager){
    size_t i=0;

    while(i<manager->block_count-1){
        Block *current=&manager->blocks[i];
        Block *next=&manager->blocks[i+1];

        if(!current->is_allocated&&!next->is_allocated){
            current->size+=next->size;

            for(size_t j=i+1;j<manager->block_count-1;j++){
                manager->blocks[j]=manager->blocks[j+1];
            }

            manager->block_count--;
        }
        else{
            i++;
        }
    }
}

int release_memory(MemoryManager *manager,uint64_t allocation_id){
    if(allocation_id==0){
        printf("Invalid allocation ID.\n");
        return 0;
    }

    int index=find_allocation(manager,allocation_id);

    if(index==-1){
        printf("Invalid allocation ID or already released.\n");
        return 0;
    }

    manager->blocks[index].is_allocated=0;
    manager->blocks[index].allocation_id=0;

    merge_free_blocks(manager);
    manager->last_failed_request=0;

    printf("Allocation released.\n");

    return 1;
}

void reserve_destination(MemoryManager *manager,int index,size_t requested_size,uint64_t allocation_id){
    size_t start=manager->blocks[index].start;
    size_t original_size=manager->blocks[index].size;

    if(original_size==requested_size){
        manager->blocks[index].is_allocated=1;
        manager->blocks[index].allocation_id=allocation_id;
        return;
    }

    for(size_t i=manager->block_count;i>(size_t)index+1;i--){
        manager->blocks[i]=manager->blocks[i-1];
    }

    manager->blocks[index].start=start;
    manager->blocks[index].size=requested_size;
    manager->blocks[index].allocation_id=allocation_id;
    manager->blocks[index].is_allocated=1;

    manager->blocks[index+1].start=start+requested_size;
    manager->blocks[index+1].size=original_size-requested_size;
    manager->blocks[index+1].allocation_id=0;
    manager->blocks[index+1].is_allocated=0;

    manager->block_count++;
}

int resize_memory(MemoryManager *manager,uint64_t allocation_id,size_t new_size){
    if(new_size==0||new_size>manager->pool_size){
        printf("Invalid resize request.\n");
        return 0;
    }

    int index=find_allocation(manager,allocation_id);

    if(index==-1){
        printf("Invalid allocation ID.\n");
        return 0;
    }

    size_t old_start=manager->blocks[index].start;
    size_t old_size=manager->blocks[index].size;

    if(new_size==old_size){
        printf("Allocation size unchanged.\n");
        return 1;
    }

    if(new_size<old_size){
        size_t released_size=old_size-new_size;

        if((size_t)index+1<manager->block_count && !manager->blocks[index+1].is_allocated){
            manager->blocks[index].size=new_size;
            manager->blocks[index+1].start=old_start+new_size;
            manager->blocks[index+1].size+=released_size;
        }
        else{
            if(!ensure_capacity(manager)){
                printf("Resize failed.\n");
                return 0;
            }

            for(size_t i=manager->block_count;i>(size_t)index+1;i--){
                manager->blocks[i]=manager->blocks[i-1];
            }

            manager->blocks[index].size=new_size;
            manager->blocks[index+1].start=old_start+new_size;
            manager->blocks[index+1].size=released_size;
            manager->blocks[index+1].allocation_id=0;
            manager->blocks[index+1].is_allocated=0;
            manager->block_count++;
        }

        manager->last_failed_request=0;
        printf("Allocation resized successfully.\n");

        return 1;
    }

    size_t additional_size=new_size-old_size;

    if((size_t)index+1<manager->block_count){
        Block *next=&manager->blocks[index+1];

        if(!next->is_allocated&&next->size>=additional_size){
            if(next->size==additional_size){
                for(size_t i=(size_t)index+1;i<manager->block_count-1;i++){
                    manager->blocks[i]=manager->blocks[i+1];
                }

                manager->block_count--;
            }
            else{
                next->start+=additional_size;
                next->size-=additional_size;
            }

            manager->blocks[index].size=new_size;
            manager->last_failed_request=0;

            printf("Allocation resized successfully.\n");

            return 1;
        }
    }

    int destination=find_suitable_block(manager,new_size);

    if(destination==-1){
        printf("Resize failed: no contiguous free block is large enough.\n");
        return 0;
    }

    size_t new_start=manager->blocks[destination].start;
    size_t destination_size=manager->blocks[destination].size;

    if(destination_size>new_size){
        if(!ensure_capacity(manager)){
            printf("Resize failed.\n");
            return 0;
        }
    }

    reserve_destination(manager,destination,new_size,allocation_id);

    memcpy(manager->pool+new_start,manager->pool+old_start,old_size);

    for(size_t i=0;i<manager->block_count;i++){
        if(manager->blocks[i].start==old_start && manager->blocks[i].allocation_id==allocation_id && manager->blocks[i].is_allocated){
            manager->blocks[i].is_allocated=0;
            manager->blocks[i].allocation_id=0;
            break;
        }
    }

    merge_free_blocks(manager);
    manager->last_failed_request=0;

    printf("Allocation resized successfully.\n");

    return 1;
}

void display_memory_map(MemoryManager *manager){
    printf("Memory Map (%zu bytes):\n",manager->pool_size);

    for(size_t i=0;i<manager->block_count;i++){
        Block *block=&manager->blocks[i];

        if(block->is_allocated){
            printf("[Allocated #%" PRIu64 ": %zu bytes, Start: %zu]\n",
                   block->allocation_id,block->size,block->start);
        }
        else{
            printf("[Free: %zu bytes, Start: %zu]\n",
                   block->size,block->start);
        }
    }
}

void display_statistics(MemoryManager *manager){
    size_t allocated;
    size_t free_memory;
    size_t free_blocks;
    size_t largest_free;
    size_t allocated_blocks;

    calculate_memory(manager,&allocated,&allocated_blocks,&free_memory,&free_blocks,&largest_free);

    printf("Total memory: %zu bytes\n",manager->pool_size);
    printf("Allocated: %zu bytes\n",allocated);
    printf("Free: %zu bytes\n",free_memory);
    printf("Free blocks: %zu\n",free_blocks);
    printf("Largest free block: %zu bytes\n",largest_free);

    if(manager->last_failed_request>0 && free_memory>=manager->last_failed_request && largest_free<manager->last_failed_request){
        printf("Note: The request for %" PRIu64" bytes failed even though %zu bytes are free, because the free memory is fragmented.\n",manager->last_failed_request,free_memory);
    }
}

void change_strategy(MemoryManager *manager,int initial){
    uint64_t choice;

    if(initial){
        printf("Select allocation strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): ");
    }
    else{
        printf("Select allocation strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): ");
    }

    if(!read_unsigned("",&choice)){
        if(!input_eof){
            printf("Invalid strategy.\n");
        }
        return;
    }

    if(choice<FIRST_FIT||choice>WORST_FIT){
        printf("Invalid strategy.\n");
        return;
    }

    manager->strategy=(int)choice;

    if(manager->strategy==FIRST_FIT){
        printf("Strategy set to First Fit.\n");
    }
    else if(manager->strategy==BEST_FIT){
        printf("Strategy set to Best Fit.\n");
    }
    else{
        printf("Strategy set to Worst Fit.\n");
    }
}

void display_menu(void){
    printf("Menu:\n");
    printf("1. Allocate memory\n");
    printf("2. Release memory\n");
    printf("3. Resize allocation\n");
    printf("4. Display memory map\n");
    printf("5. Display memory statistics\n");
    printf("6. Change allocation strategy\n");
    printf("7. Exit\n");
}

void cleanup(MemoryManager *manager)
{
    free(manager->pool);
    free(manager->blocks);

    manager->pool=NULL;
    manager->blocks=NULL;
    manager->block_count=0;
    manager->capacity=0;
}

int main(void)
{
    MemoryManager manager={0};
    uint64_t input;

    printf("Enter memory pool size: ");

    if(!read_unsigned("",&input)){
        if(!input_eof){
            printf("Invalid pool size.\n");
        }
        return input_eof?0:1;
    }

    if(input<1||input>MAX_POOL_SIZE){
        printf("Invalid pool size.\n");
        return 1;
    }

    if(!initialize_pool(&manager,(size_t)input)){
        return 1;
    }

    while(manager.strategy==0){
        change_strategy(&manager,1);

        if(input_eof){
            cleanup(&manager);
            return 0;
        }
    }

    while(1){
        display_menu();

        if(!read_unsigned("Choice: ",&input)){
            if(input_eof){
                cleanup(&manager);
                return 0;
            }

            printf("Invalid choice.\n");
            continue;
        }

        switch(input){
            case 1:
            {
                uint64_t requested_size;

                if(!read_unsigned("Requested size: ",&requested_size)){
                    if(input_eof){
                        cleanup(&manager);
                        return 0;
                    }

                    printf("Invalid size.\n");
                    break;
                }

                allocate_memory(&manager,(size_t)requested_size);

                break;
            }

            case 2:
            {
                uint64_t allocation_id;

                if(!read_unsigned("Allocation ID: ",&allocation_id)){
                    if(input_eof){
                        cleanup(&manager);
                        return 0;
                    }

                    printf("Invalid ID.\n");
                    break;
                }

                release_memory(&manager,allocation_id);

                break;
            }

            case 3:
            {
                uint64_t allocation_id;
                uint64_t new_size;

                if(!read_unsigned("Allocation ID: ",&allocation_id)){
                    if(input_eof){
                        cleanup(&manager);
                        return 0;
                    }

                    printf("Invalid ID.\n");
                    break;
                }

                if(!read_unsigned("New size: ",&new_size)){
                    if(input_eof){
                        cleanup(&manager);
                        return 0;
                    }

                    printf("Invalid size.\n");
                    break;
                }

                resize_memory(&manager,allocation_id,(size_t)new_size);

                break;
            }

            case 4:
            {
                display_memory_map(&manager);
                break;
            }

            case 5:
            {
                display_statistics(&manager);
                break;
            }

            case 6:
            {
                change_strategy(&manager,0);

                if(input_eof){
                    cleanup(&manager);
                    return 0;
                }

                break;
            }

            case 7:
            {
                cleanup(&manager);
                return 0;
            }

            default:
            {
                printf("Invalid choice.\n");
            }
        }
    }

    return 0;
}
