//
// Created by Daniel Williams on 2019-06-24.
//

#include <stdlib.h>
#include <stdio.h>

void DisplayBits(unsigned value){
    unsigned c, displayMask = 1 << 15;

    //printf("%3c%7u = ", value, value);

    for(c=1; c<=16; c++){
        putchar(value & displayMask ? '1' : '0');
        value <<= 1;

        if (c % 8 == 0){
            putchar(' ');
        }
    }
    putchar('\n');
}


// Returns a 16 bit integer that is holding the packed characters
unsigned int PackCharacters(char a, char b);

unsigned int PackCharacters(char a, char b) {

    DisplayBits(a);
    DisplayBits(b);
    // Assign the first character to the to the unsigned variable
    // Shift the unsigned variable left by 8 bit positions
    unsigned int new = a;
    new <<= 8;
    // Combine the unsigned variable with the second character using the bitwise inclusive OR operator
    new |= b;

    DisplayBits(new);
    return new;
}

// Using the right shift operator, the bitwise AND (&&)operator, and a mask
void UnpackCharacters(unsigned int packedInt);

void UnpackCharacters(unsigned int packedInt){
    DisplayBits(packedInt);

    unsigned int first_letter;
    first_letter = packedInt >> 8;
    unsigned int second_letter;
    second_letter = packedInt << 8;
    second_letter >>= 8;

    printf("%c", first_letter);
    putchar('\n');
    printf("%c", second_letter);
    putchar('\n');
}

int Power2(int num, int pow);

int Power2(int num, int pow){
    int multiple = num;
    multiple <<= pow;
    return multiple;
}


int main(void) {
    UnpackCharacters(PackCharacters('B', 'i'));
    int multiple = Power2(2, 2);
    printf("%d", multiple);
    putchar('\n');
    DisplayBits(multiple);
    return 0;
}