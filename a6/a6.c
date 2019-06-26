//
// Created by Daniel Williams on 2019-06-24.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dictionary.h"

/**
 *. Feel free to modify this file and function prototypes 
 * to more efficiently print out the data you desire. 
 * 
 * We *will* run this by calling "make benchmark", 
 * so be sure that works. 
 */


void BenchmarkArray(char* filename) {
    char c[kMaxWordLen];

    FILE *cfPtr;
    if ((cfPtr = fopen(filename, "r")) == NULL) {
        printf("File could not be opened\n");
    }

    while (fscanf(cfPtr,"%s", c) == 1) {
        printf("%s\n", c);
    }
    fclose(cfPtr); //close dict file
}


void BenchmarkTrie(char* filename) {
    FILE *cfPtr;

    if ((cfPtr = fopen(filename, "r")) == NULL) {
        printf("File could not be opened\n");
    } else {
        char c;

        while (!feof(cfPtr)) {
            // Reads file character by character
            c = fgetc(cfPtr);
            printf("%c\n", c);
        }
        fclose(cfPtr);
    }
}

int main() {
  BenchmarkArray("wordlist_small.txt");
  BenchmarkTrie("wordlist_small.txt");
  return 0;
}
