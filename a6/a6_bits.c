//
// Created by Daniel Williams on 2019-06-24.
//

#include <stdlib.h>
#include <stdio.h>

void DisplayBits(unsigned value) {
    unsigned c, displayMask = 1 << 15;

    for (c = 1; c <= 16; c++) {
        putchar(value & displayMask ? '1' : '0');
        value <<= 1;

        if (c % 8 == 0) {
            putchar(' ');
        }
    }
    putchar('\n');
}


// Returns a 16 bit integer that is holding the packed characters
unsigned int PackCharacters(char a, char b);

unsigned int PackCharacters(char a, char b) {
    printf("\nUnpacked:\n");
    DisplayBits(a);
    DisplayBits(b);
    unsigned int new = a;
    new <<= 8;
    new |= b;
    printf("\nPacked:\n");
    return new;
}

void UnpackCharacters(unsigned int packedInt);

void UnpackCharacters(unsigned int packedInt) {
    DisplayBits(packedInt);

    unsigned int first_letter;
    first_letter = packedInt >> 8;
    unsigned int second_letter;
    second_letter = packedInt << 8;
    second_letter >>= 8;
    printf("\nFirst Letter:\n");
    printf("%c", first_letter);
    putchar('\n');
    printf("\nSecond Letter:\n");
    printf("%c", second_letter);
    putchar('\n');
}

int Power2(int num, int pow) {
    int multiple = num;
    multiple <<= pow;
    printf("\nInteger:\n");
    printf("%d", multiple);
    putchar('\n');
    printf("\nBits:\n");
    DisplayBits(multiple);
    printf("\n");
    return multiple;
}

int main(void) {
    UnpackCharacters(PackCharacters('B', 'i'));
    int multiple = Power2(2, 2);
    return 0;
}