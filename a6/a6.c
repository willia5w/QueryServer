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

    clock_t start = clock(), diff;
    DictArray* dict = LoadDictionaryArray(filename);
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken to load array: %d seconds %d milliseconds", msec/1000, msec%1000);
}


void BenchmarkTrie(char* filename) {
    clock_t start = clock(), diff;

    DictTrie* dict = LoadDictionaryTrie(filename);

    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("\nTime taken to load trie: %d seconds %d milliseconds", msec/1000, msec%1000);
}

int main() {
  BenchmarkArray("wordlist.txt");
  BenchmarkTrie("wordlist.txt");
  return 0;
}

