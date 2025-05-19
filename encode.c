#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_BYTES 256

typedef struct Byte {
    unsigned char byte;
    unsigned int freq;
    char *code;
    unsigned int code_size;
    struct Byte *left;
    struct Byte *right;
} Byte;

void print_bytes(Byte *array[], size_t size);
int compare_bytes(const void *b1, const void *b2);
void insert_byte(Byte *byte, Byte *array[], size_t size,int pos);

int main(int argc, char *argv[]) {
    // initialize bytes
    Byte bytes[NUM_BYTES];
    for(unsigned int i = 0; i < NUM_BYTES; i++) {
        bytes[i].byte = (unsigned char) i;
        bytes[i].freq = 0;
        bytes[i].code = "";
        bytes[i].code_size = 0;
    }

    // read bytes and update frequencies
    FILE *input = fopen(argv[1], "r");
    if(!input) {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }
    unsigned char temp;
    while(fread(&temp, 1, 1, input) != 0) {
        bytes[temp].freq += 1;
    }
    fclose(input);

    // array of pointers to bytes sorted by frequencies
    Byte *sorted_bytes[NUM_BYTES];
    for(int i = 0; i < NUM_BYTES; i++) {
        sorted_bytes[i] = bytes + i;
    }
    qsort(sorted_bytes, NUM_BYTES, sizeof(Byte*), compare_bytes);
    print_bytes(sorted_bytes, NUM_BYTES);

    
    // printf("%zu\n", sizeof(sorted_bytes));
    return 0;
}

void print_bytes(Byte *array[], size_t size) {
    for(int i = 0; i < size; i++) {
        Byte *temp = array[i]; 
        printf("%02X: %u\n", temp->byte, temp->freq);
    }
}

int compare_bytes(const void *b1, const void *b2) {
    const Byte *const *byte1 = b1;
    const Byte *const *byte2 = b2;

    unsigned int f1 = (*byte1)->freq;
    unsigned int f2 = (*byte2)->freq;

    if (f1 < f2) return -1;
    if (f1 > f2) return 1;
    return 0;
}

void insert_byte(Byte *byte, Byte *array[], size_t size,int pos) {
    for(int i = size - 1; i > pos; i--) {
        array[i] = array[i-1];
    }
    array[pos] = byte;
}