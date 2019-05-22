#include <stdio.h>
#include "sort.h"
#include <stdlib.h>
#include <time.h>

#define SMALL_SIZE  4
#define MED_SIZE  128
#define LARGE_SIZE  1024


int main() {
  int small[SMALL_SIZE];
  int medium[MED_SIZE];
  int large[LARGE_SIZE];

  srand(time(0));

  int i;
  for (i = 0; i < SMALL_SIZE; i++) {
      small[i] = rand();
  }  
  for (i = 0; i < MED_SIZE; i++) {
      medium[i] = rand();
  }
  for (i = 0; i < LARGE_SIZE; i++) {
      large[i] = rand();
  }
  int l = Sort(large, LARGE_SIZE);
  int m = Sort(medium, MED_SIZE);
  int s = Sort(small, SMALL_SIZE);

  printf("---------------------\nSORTING:\n---------------------\n");
  printf("Large: 1024 elements %d comparisons\n", l);
  printf("Med: 128 elements %d comparisons\n", m);
  printf("Small: 4 elements %d comparisons\n", s);
  
  return 0;
}
