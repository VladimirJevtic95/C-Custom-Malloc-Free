#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

typedef struct
{
    int is_available;
    int size;
} MEMORY_BLOCK, * MEMORY_BLOCK_POINTER;


//integral constants
enum { NEW_MEMORY_BLOCK = 0, NUMBER_MEMORY_BLOCK, REUSE_MEMORY_BLOCK };
enum { FREE, IN_USE };


char* heap_end; //pointer to a character
char* memory_start_pointer;  //pointer to a character
int max_memory; //max memory
int allocated_memory; //memory in use
int memory_block_count; //count of memory blocks

void memory_init(char* pointer, int size_in_bytes) {
    // store the pointer in global variable-> max_memory and size_in_bytes in global variable-> memory_start_pointer
    max_memory = size_in_bytes;
    memory_start_pointer = pointer;
    memory_block_count = 0;
    allocated_memory = 0;
    heap_end = memory_start_pointer + size_in_bytes;
}


void* memory_alloc(int element_size) {
    //check if any block of memory that was alocated before is free
    MEMORY_BLOCK_POINTER pointer_memory_block;
    int flag = NUMBER_MEMORY_BLOCK;

    pointer_memory_block = (MEMORY_BLOCK_POINTER)memory_start_pointer;

    int block_size;

    block_size = sizeof(MEMORY_BLOCK);

    //check if max size is exceeded
    if ((element_size + block_size) > (max_memory - (allocated_memory + memory_block_count * block_size)))
    {
        printf("PREVAZIDJENA MAX VELICINA\n");
        return NULL;
    }
    while (heap_end > ((char*)pointer_memory_block + element_size + block_size))
    {
        //check if the assigned value to the is_available variable is 0
        if (pointer_memory_block->is_available == 0)
        {
            //check if the assigned value to the size variable is 0
            if (pointer_memory_block->size == 0)
            {
                flag = NEW_MEMORY_BLOCK;
                break;
            }
            //check if the assigned value to the size variable of pointer_memory_block is greater than element and block size
            if (pointer_memory_block->size > (element_size + block_size))
            {
                flag = REUSE_MEMORY_BLOCK;
                break;
            }
        }
        pointer_memory_block = (MEMORY_BLOCK_POINTER)((char*)pointer_memory_block + pointer_memory_block->size);
    }

    if (flag != NUMBER_MEMORY_BLOCK)
    {
        //assigning value (1) to the is_available variable of pointer_memory_block
        pointer_memory_block->is_available = 1;

        if (flag == NEW_MEMORY_BLOCK)
        {
            //assigning the value to the size variable of pointer_memory_block
            pointer_memory_block->size = element_size + sizeof(MEMORY_BLOCK);
            memory_block_count++;
        }
        allocated_memory += element_size;
        return ((char*)pointer_memory_block + block_size);
    }

    printf("KRAJ, MEMORIJA U MEMORIJSKOM BLOKU NE MOZE BITI ALOCIRANA\n");
    return NULL;
    //if the size of the available block is equal or greater than requiered block size, use that block
}

int memory_check_efficiency() {
    //keep track of the number of memory blocks in global variable
    return memory_block_count;
}

void memory_free(void* pointer_free) {
    printf("\n------------------------------------------------------\n");
    printf("FREE funkcija POCETAK");

    //obelezi u bloku memorije da je ovaj blok slobodan
    MEMORY_BLOCK_POINTER memory_block_pointer = (MEMORY_BLOCK_POINTER)pointer_free;
    memory_block_pointer--;
    memory_block_count--;
    //assigning FREE value to the is_available variable of memory_block_pointer
    memory_block_pointer->is_available = FREE;
    //printing the assigned value to the variable
    printf("\nAlocirana memorije za FREE: %d ", memory_block_pointer->size);
    printf("\nMemorija oslobodjena -> FREE");
    allocated_memory -= (memory_block_pointer->size - sizeof(MEMORY_BLOCK));
    printf("\nAlocirana memorija ostalo: %d ", allocated_memory);

    printf("\nFREE funkcija KRAJ");
    printf("\n------------------------------------------------------\n");
}

int main()
{
    int regionMem;
    int memoryToAlocate, memoryToAlocate1, memoryToAlocate2;
    char* string, * string1, * string2;

    char region[1024];
    //without memset we cant allocate memory in the memory block
    //and it is used to fill a block of memory with a particular value
    //in order (I-starting adress of memory to be filled, 
    //II-value to be filled, III-number of bytes to be filled starting from region to be filled)
    memset(region, 0, 1024);
    memory_init(region, 1024);



    printf("Kolicina memorije koju hocemo da alociramo : ");
    scanf("%d", &memoryToAlocate);
    string = memory_alloc(memoryToAlocate);
    printf("\nAdresa memorije: %p", string);
    printf("\nBroj memorijskih blokova: %-3d \tAlocirana memorija: %-10d", memory_block_count, allocated_memory);
    memory_free(string);
    printf("\n**Broj alociranih memorijskih blokova: %-3d\n**Ukupna alocirana memorija posle oslobadjanja %d mesta u memoriji : %-10d\n", memory_block_count, memoryToAlocate, allocated_memory);
    printf("\n--------------------------------------------------------------------------------------------------------");
    printf("\n--------------------------------------------------------------------------------------------------------\n");

    printf("Unesite vrenodst za MEMORIJU 1 koju alociramo : ");
    scanf("%d", &memoryToAlocate1);
    string1 = memory_alloc(memoryToAlocate1);
    printf("Unesite vrenodst za MEMORIJU 2 koju alociramo : ");
    scanf("%d", &memoryToAlocate2);
    string2 = memory_alloc(memoryToAlocate2);
    printf("\n\nAdresa memorije za MEMORIJU 1: %p", string1);
    printf("\nAdresa memorije za MEMORIJU 2: %p", string2);
    printf("\nBroj alociranih memorijskih blokova: %-3d \tUkupna alocirana memorija: %-10d\n", memory_block_count, allocated_memory);
    printf("***Oslobadjamo %d mesta u memoriji.", memoryToAlocate1);
    memory_free(string1);
    printf("\n**Broj alociranih memorijskih blokova: %-3d\n**Ukupna alocirana memorija posle oslobadjanja %d mesta u memoriji : %-10d\n", memory_block_count, memoryToAlocate1, allocated_memory);


}