#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_BYTES 256
#define TREE_SIZE 1024

typedef struct Byte {
    char* data;
    int data_size;
    char *code;
    unsigned int code_size;
    unsigned int freq;
    struct Byte *left;
    struct Byte *right;
} Byte;

void print_bytes(Byte *array[], int from, int to);
int compare_bytes(const void *b1, const void *b2);
void insert_byte(Byte *byte, Byte *array[], size_t size,int pos);
void update_children_code(Byte* parent, int val);


int main(int argc, char *argv[]) {
    // initialize bytes 
    Byte bytes[NUM_BYTES];
    for(unsigned int i = 0; i < NUM_BYTES; i++) {
        bytes[i].data = malloc(1);
        memset(bytes[i].data, (char) i, 1);
        bytes[i].data_size = 1;
        bytes[i].code = malloc(0);
        bytes[i].code_size = 0;
        bytes[i].freq = 0;
        bytes[i].left = NULL;
        bytes[i].right = NULL;
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
    // print_bytes(sorted_bytes, NUM_BYTES);

    // big array to build tree
    // first 256 are sorted_bytes
    // rest are mallocd but NULL
    Byte *tree[TREE_SIZE];
    memcpy(tree, sorted_bytes, NUM_BYTES*sizeof(Byte*));
    for(int i = NUM_BYTES; i < TREE_SIZE; i++) {
        tree[i] = malloc(sizeof(Byte));
        tree[i]->data = malloc(0);
        tree[i]->data_size = 0;
        tree[i]->code = malloc(0);
        tree[i]->code_size = 0;
        tree[i]->freq = 0;
        tree[i]->left = NULL;
        tree[i]->right = NULL;
    } 
    // print_bytes(sorted_bytes, NUM_BYTES);
    // print_bytes(tree, TREE_SIZE); 
    
    int first_non_zero;
    for(int i = 0; i < TREE_SIZE; i++) {
        if(tree[i]->freq > 0) {
            first_non_zero = i;
            break;
        }
    }

    // Byte x = {
    //     .data = "abc",
    //     .data_size = 3,
    //     .code = "101",
    //     .code_size = 3,
    //     .freq = 69,
    //     .left = NULL,
    //     .right = NULL
    // };

    // insert_byte(&x, tree, TREE_SIZE, first_non_zero + 2);

    for(int i = first_non_zero; i < TREE_SIZE -1 ; i+=2) {
        //read 2 bytes
        Byte *b1, *b2;
        b1 = tree[i];
        b2 = tree[i+1];
        //check if not null
        if(b1->data == NULL || b2->data == NULL) {
            break;
        }
        //append to code
        b1->code_size += 1;
        b2->code_size += 1;
        b1->code = malloc(b1->code_size);
        b2->code = malloc(b2->code_size);
        memcpy(b1->code+1, b1->code, b1->code_size - 1);
        memset(b1->code, 0, 1);
        memcpy(b2->code+1, b2->code, b2->code_size - 1);
        memset(b2->code, 1, 1);
        
        //check for children and append to their code
        update_children_code(b1, 0);
        update_children_code(b2, 1);

        //combine b1 and b2 into parent
        Byte *parent = malloc(sizeof(Byte));
        parent->data_size = b1->data_size + b2->data_size;
        parent->data = malloc(parent->data_size);
        memmove(parent->data, b1->data, b1->data_size);
        memmove(parent->data + b1->data_size, b2->data, b2->data_size);
        parent->code = malloc(0);
        parent->code_size = 0;
        parent->freq = b1->freq + b2->freq;
        parent->left = b1;
        parent->right = b2;

        //insert parent into tree
        int pos;
        for(int i = first_non_zero; i < TREE_SIZE; i++) {
            if(parent->freq < tree[i]->freq) {
                pos = i;
                break;
            }
        }
        // printf("%d\n", pos);
        insert_byte(parent, tree, TREE_SIZE, pos);
    }

    print_bytes(tree, first_non_zero, TREE_SIZE);
    
    // printf("%zu\n", sizeof(sorted_bytes));
    return 0;
}

void print_bytes(Byte *array[], int from, int to) {
    for(int i = from; i < to; i++) {
        Byte *temp = array[i]; 
        printf("%d. data: ", i);
        for(int j = 0; j < temp->data_size; j++) {
            printf("%02x ", (unsigned char)temp->data[j]);
        }
        printf("code: ");
        for(int j = 0; j < temp->code_size; j++) {
            printf("%x", (char)temp->code[j]);
        }
        printf(" freq: %d\n", temp->freq);
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

void insert_byte(Byte *byte, Byte *array[], size_t size, int pos) {
    // for(int i = size - 1; i > pos; i--) {
    //     array[i] = array[i-1];
    // }
    memmove(&(array[pos + 1]), &(array[pos]) , (size - pos - 1)*sizeof(Byte*));
    array[pos] = byte;
}

void update_children_code(Byte* parent, int val) {
    if(parent->left == NULL && parent->right ==  NULL) {
        return;
    } else {
        if(parent->left != NULL) {
            Byte *left = parent->left;
            left->code_size += 1;
            realloc(left->code, left->code_size);
            memmove(left->code + 1, left->code, left->code_size-1);
            memset(left->code, val, 1);
            update_children_code(left, val);
        }
        if(parent->right != NULL) {
            Byte *right = parent->right;
            right->code_size += 1;
            realloc(right->code, right->code_size);
            memmove(right->code + 1, right->code, right->code_size-1);
            memset(right->code, val, 1);
            update_children_code(right, val);
        }        
    }
}