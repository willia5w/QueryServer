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
        FILE *cfPtr;

        if ((cfPtr = fopen("sample.txt", "r")) == NULL) {
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

        int main(){
            ReadFile();
            return 0;
        }
}

//void BenchmarkTrie(char* filename) {
  

//}

//char lookup(char word, BenchmarkArray(char* filename));

int main() {
  BenchmarkArray("wordlist.txt");
  //BenchmarkTrie("wordlist.txt");
}
