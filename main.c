#include <stdlib.h>
#include <stdio.h>
#define NUM_BYTES 256

typedef struct Byte {
    unsigned char byte;
    unsigned int freq;
} Byte;

typedef struct Node {
    unsigned char *data;
    struct Node *left;
    struct Node *right;
} Node;

int main(int argc, char *argv[]) {
    //initialize bytes
    Byte bytes[NUM_BYTES];
    for(unsigned int i = 0; i < NUM_BYTES; i++) {
        bytes[i].byte = (unsigned char) i;
        bytes[i].freq = 0;
    }

    //read bytes and update frequencies
    FILE *input = fopen(argv[1], "r");
    if(!input) {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }
    unsigned char temp;
    while(fread(&temp, sizeof(char), 1, input) != 0) {
        bytes[temp].freq += 1;
    }
    fclose(input);

    // print frequencies
    // for(unsigned int i = 0; i < NUM_BYTES; i++) {
    //     printf("%.2x: %d\n", bytes[i].byte, bytes[i].freq);
    // }

    //sorted
    for(int i = NUM_BYTES - 1; i >= 1; i--) {
        for(int j = 0; j < i; j++) {
            if(bytes[j].freq > bytes[j+1].freq) {
                Byte temp = bytes[j];
                bytes[j] = bytes[j+1];
                bytes[j+1] = temp;
            }
        }
    }

    // print frequencies
    for(unsigned int i = 0; i < NUM_BYTES; i++) {
        // printf("%.2x: %d\n", bytes[i].byte, bytes[i].freq);
    }

    unsigned int offset = 0;
    for(int i = 0; i < NUM_BYTES; i++) {
        if(bytes[i].freq > 0) {
            offset = i;
            break;
        }
    }

    Node q1[NUM_BYTES*2];
    for(int i = 0; i < NUM_BYTES-offset; i++) {
        q1[i].data = &(bytes[i+offset].byte);
        q1[i].left = NULL;
        q1[i].right = NULL;
        // printf("%.2x %.2x\n", bytes[i + offset].byte, q1[i].data[0]);
    }
    for(int i = NUM_BYTES-offset; i < NUM_BYTES*2; i++) {
        q1[i].data = "";
        q1[i].left = NULL;
        q1[i].right = NULL;
    }

    printf("%d\n",sizeof(q1)/sizeof(Node));

    return 0;
}