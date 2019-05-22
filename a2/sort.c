#include <stdio.h>

int Sort(int* array, int length) {
  int left = length / 2;
  int comparisons = 0;
  while(left > 0) {
    int q = length / 2;
    int r = 0;
    int gap = left;
    while(q >= left) {
      for(int i = 0; i < length - gap; i++) {
        if((i & left) == r) {
          comparisons++;
          if(array[i] > array[i + gap]) {
            comparisons++;
            array[i] = array[i + gap];
          }
        }
        gap = q - left;
        q = q / 2;
        r = left;
      }
    }
    left = left / 2;
  } 
  return comparisons;
  //printf("%d Comparisons Made", comparisons);
}
