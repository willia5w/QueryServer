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
  
}



void BenchmarkTrie(char* filename) {
  

}

int main() {
  BenchmarkArray("wordlist.txt");
  BenchmarkTrie("wordlist.txt"); 
}
