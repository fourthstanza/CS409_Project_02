#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PR_LENGTH 10

int MEMORY_SIZE;

typedef struct Memblock {
    int start;
    int end;
    int size;
    int allocated;
    char process[PR_LENGTH];
    struct Memblock *next;
} Memblock;


Memblock* create_block(int start, int size, int allocated, const char* process) {
    Memblock* block = (Memblock*)malloc(sizeof(Memblock));
    block->start = start;
    block->end = start + size - 1;
    block->size = size;
    block->allocated = allocated;
    strncpy(block->process, process, PR_LENGTH);
    block->next = NULL;
    return block;
}

Memblock *memory = NULL;

void initialize_memory(){
    memory = create_block(0, MEMORY_SIZE, 0, "Unused");
}

void split_block(Memblock *target, int size){
    Memblock *newBlock = create_block(target -> start + size, target -> size - size, 0, "Unused");
    newBlock -> next = target -> next;

    target -> next = newBlock;
    target -> end = target -> start + size - 1;
    target -> size = size;
}

int fits(Memblock *block, int size){
    return (!block -> allocated && block -> size >= size);
}

void allocate_memory(process, size, strategy){
    Memblock *curr = memory;
    Memblock *target = NULL;

    if (strcmp(strategy, "F") == 0){
        while (curr){
            if(fits(curr, size)){
                target = curr;
                break;
            }
            curr = curr -> next;
        }
    }
    else if (strcmp(strategy, "B") == 0){
        int bestSize = MEMORY_SIZE;
        while(curr){
            if(fits(curr,size) && curr -> size < bestSize){
                target = curr;
                bestSize = target -> size;
            }
            curr = curr -> next;
        }
    }
    else if (strcmp(strategy, "W") == 0){
        int worstSize = 0;
        while(curr){
            if(fits(curr,size) && curr -> size > worstSize){
                target = curr;
                worstSize = target -> size;
            }
            curr = curr -> next;
        }
    }
    else{
        printf("\nInvalid strategy type, please choose F, B, or W");
    }

    if (!target){
        printf("\nNo suitable open block found");
        return;
    }

    if (!target -> size == size){
        split_block(target, size);
    }
    target -> allocated = 1;
    strcpy(target -> process, process);
}

void release_memory(process){
    Memblock* curr = memory;
    while (curr){
        if (strcmp(curr -> process, process) == 0){
            curr -> allocated = 0;
            strcpy(curr -> process, "Unused");
            printf("Memory for process %s deallocated", process);
            return;
        }
        curr = curr -> next;
    }
    printf("\nProcess not found");
}

void compact_memory(){
    Memblock *curr = memory;
    Memblock *last = NULL;
    Memblock *nextBlock = NULL;

    while(curr){
        if(curr->allocated == 1){
            last = curr;
            curr = curr -> next;
        }
        else if (curr -> next){
            nextBlock = curr -> next;
            last -> next = nextBlock;
            nextBlock -> start = last -> end + 1;
            nextBlock -> end = nextBlock -> start + nextBlock -> size - 1;
            free(curr);
            curr = nextBlock;
        }
    }
    int usedEnd = last -> end;
    Memblock *unused = create_block(usedEnd + 1, MEMORY_SIZE - usedEnd - 1, 0, "Unused");
    last -> next = unused;
}

int main(int argc, char*argv[]) {

    //0B < Memory size < 1MB
    MEMORY_SIZE = atoi(argv[1]);
    if (MEMORY_SIZE < 0 || MEMORY_SIZE > 1048576){
        printf("\nInvalid memory length, defaulting to 128 bytes");
        MEMORY_SIZE = 128;
    }

    initialize_memory();

    char command[20], process[PR_LENGTH], strategy[2];
    int size;

    while (1) {
        printf("\nEnter command: ");
        scanf("%s", command);

        if (strcmp(command, "RQ") == 0) {
            scanf("%s %d %s", process, &size, strategy);
            allocate_memory(process, size, strategy);
        } else if (strcmp(command, "RL") == 0) {
            scanf("%s", process);
            release_memory(process);
        } else if (strcmp(command, "C") == 0) {
            compact_memory();
        } else if (strcmp(command, "STAT") == 0) {
            print_status();
        } else if (strcmp(command, "EXIT") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}