//
// Created by Daniel Williams on 2019-06-24.
//

#include <stdlib.h>
#include <stdio.h>



int GetBit(unsigned short int, int index);

void ToggleBit(unsigned short int* bitstring, int which_bit);

// Assumes which_bit is < the num bits in unsigned int
void ToggleBit(unsigned short int* bitstring, int which_bit) {
    *bitstring ^= 1 << which_bit;
}

int GetBit(unsigned short int bitstring, int which_bit) {
    return (bitstring >> which_bit) & 1U;
}

void DisplayBits(unsigned value){
    unsigned c, displayMask = 1 << 15;

    printf("%3c%7u = ", value, value);

    for(c=1; c<=16; c++){
        putchar(value & displayMask ? '1' : '0');
        value <<= 1;

        if (c % 8 == 0){
            putchar(' ');
        }
    }
    putchar('\n');
}

int main(void) {
    /* initialize empty array with the right size */
    unsigned short int x = 0;

    printf("An unsigned short int has %lu bytes. \n", sizeof(unsigned short int));

    int i;

    ToggleBit(&x, 3);
    ToggleBit(&x, 4);
    ToggleBit(&x, 6);
    ToggleBit(&x, 7);
    ToggleBit(&x, 12);

    DisplayBits(x);

    printf("Bit 12 = %d \n", GetBit(x, 12));
    ToggleBit(&x, 12);
    DisplayBits(x);

    printf("\n\n");

    DisplayBits('B');
    DisplayBits('i');
    DisplayBits('k');

    return 0;
}