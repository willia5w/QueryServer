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
<<<<<<< HEAD

    clock_t start = clock(), diff;
    DictArray* dict = LoadDictionaryArray(filename);
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken to load array: %d seconds %d milliseconds", msec/1000, msec%1000);
=======
    char c[kMaxWordLen];

    FILE *cfPtr;
    if ((cfPtr = fopen(filename, "r")) == NULL) {
        printf("File could not be opened\n");
    }

    while (fscanf(cfPtr,"%s", c) == 1) {
        printf("%s\n", c);
    }
    fclose(cfPtr); //close dict file
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
}


void BenchmarkTrie(char* filename) {
<<<<<<< HEAD
    clock_t start = clock(), diff;

    DictTrie* dict = LoadDictionaryTrie(filename);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("\nTime taken to load trie: %d seconds %d milliseconds", msec/1000, msec%1000);
}

int main() {
  BenchmarkArray("wordlist.txt");
  BenchmarkTrie("wordlist.txt");
=======
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
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
  return 0;
}

