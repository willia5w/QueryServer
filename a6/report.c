//
// Created by Daniel Williams on 2019-06-24.
//

#include <time.h>

clock_t start = clock(), diff;
ProcessIntenseFunction();
diff = clock() - start;

int msec = diff * 1000 / CLOCKS_PER_SEC;
printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);


#include <stdio.h>

void ReadFile(){
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