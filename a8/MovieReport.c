// Modified by Dan Williams 7/24/2019
// Thank you Adrienne for your Piazza post and blessing to use this code

#include <stdio.h>
#include <stdlib.h>

#include "MovieIndex.h"
#include "MovieReport.h"
#include "Movie.h"
#include "MovieSet.h"
#include "htll/LinkedList.h"
#include "htll/Hashtable.h"


// Prints a report of an index, assuming the key is a field/string and the value is a MovieSet.
void PrintReport(Index index) {
  OutputReport(index, stdout);
}

void OutputMovieList(LinkedList movie_list, char *desc, FILE *file) {
  fprintf(file, "%s: %s\n", "indexType", desc);
  fprintf(file, "%d items\n", NumElementsInLinkedList(movie_list));
  if (NumElementsInLinkedList(movie_list) == 0) {
    return;
  }
  LLIter iter = CreateLLIter(movie_list);
  if (iter == NULL) {
    fprintf(file, "iter null for some reason.. \n");
    return;
  }
  Movie *movie;

  LLIterGetPayload(iter, (void**)&movie);
  if (movie->title != NULL) fprintf(file, "\t%s\n", movie->title);
  else fprintf(file, "title is null\n");
 
  while (LLIterHasNext(iter)) {
    LLIterNext(iter);
    LLIterGetPayload(iter, (void**)&movie);
    if (movie->title != NULL) fprintf(file, "\t%s\n", movie->title);
    else fprintf(file, "title is null\n");
  }

  DestroyLLIter(iter);
}

// Assumes the value of the index is a MovieSet
void OutputReport(Index index, FILE* output) {
  HTIter iter = CreateHashtableIterator(index->ht);
  HTKeyValue movie_set;

  HTIteratorGet(iter, &movie_set);

  OutputListOfMovies(((MovieSet)movie_set.value)->movies,
                     ((MovieSet)movie_set.value)->desc,
                     output);

  while (HTIteratorHasMore(iter)) {
    HTIteratorNext(iter);
    HTIteratorGet(iter, &movie_set);
    OutputListOfMovies(((MovieSet)movie_set.value)->movies,
                       ((MovieSet)movie_set.value)->desc,
                       output);
  }

  DestroyHashtableIterator(iter);
}

void SaveReport(Index index, char* filename) {
 // Open file
  FILE * fp;
  fp = fopen(filename, "w+");

  // Pass to OutputReport
  OutputReport(index, fp);

  // Close file
  fclose(fp); 
}


