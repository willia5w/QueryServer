/*
 *  Modified by Daniel Williams 8/6/2019
 *  Created by Adrienne Slaughter
 *  CS 5007 Summer 2019
 *  Northeastern University, Seattle
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  See <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#include "DirectoryParser.h"
#include "MovieTitleIndex.h"
#include "Movie.h"
#include "DocIdMap.h"


#define BUFFER_SIZE 1000

//=======================
// To minimize the number of files we have, I'm
// putting the private function prototypes for
// the DirectoryParser here.

/**
 * Helper function to index a single file. 
 *
 * \return a pointer to the number of records (lines) indexed from the file
 */
void* IndexAFile_MT(void *toBeIter);

pthread_mutex_t ITER_MUTEX = PTHREAD_MUTEX_INITIALIZER;  // global variable
pthread_mutex_t INDEX_MUTEX = PTHREAD_MUTEX_INITIALIZER;  // global variable

int MORE = 0;  // Set to 1 if iterator has no more, lock to limit to one thread

// THINK: Why is this global?
MovieTitleIndex movieIndex;

/**
 * Given a map of all the files that we want to index
 * and search, open each file and index the contents to index
 *
 * \param docs the DocIdMap that contains all the files we want to parse.
 * \param the index to hold all the indexed docs.
 * \param num_threads The number of threads to use to parse the files
 *
 * \return The number of records parsed
 */
int ParseTheFiles_MT(DocIdMap docs, MovieTitleIndex index, int num_threads) {
  int records_parsed = 0;  // Increment with value returned by IndexAFile_MT
  movieIndex = index;  // Synchronize index with global movie index

  clock_t start, end;
  double cpu_time_used;
  start = clock();

  DocIdIter docname_iter = CreateDocIdIterator(docs);
  if (docname_iter == NULL) {
    return -1;
  }

  pthread_t threads[num_threads];  // Store current threads
  void * num_records[num_threads];  // Values representing records parsed by each thread

  // for loop to create threads
  for (int i = 0; i < num_threads; i++) {  // Create number of threads specified
    if (pthread_create(&threads[i], NULL, IndexAFile_MT, (void*)docname_iter) != 0) {  // Create p_thread
      fprintf(stderr, "p_thread %d\n create error", i);
      break;
    }
  }
  // for loop to join all threads and get counts
  for (int j = 0; j < num_threads; j++) {
    if (pthread_join(threads[j], &num_records[j]) != 0) {  // Set num_records);
      fprintf(stderr, "Thread %d\n join error", j);
      break;
    }
    records_parsed += *((int*)num_records[j]);  // Increment number of records
    free(num_records[j]);  // Free num_records pointer
  }
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Took %f seconds to execute. \n", cpu_time_used);
  DestroyDocIdIterator(docname_iter);  // Free HT iterator
  MORE = 0;
  return 0;  // Get number of records parsed
}
void* IndexAFile_MT(void *docname_iter) {
  int* records = (int*)malloc(sizeof(int));
  *records = 0;  // Counter for files indexed
  HTIter iter = (HTIter)docname_iter;  // Don't forget to free this at some point!!
  HTKeyValue kvp;  // HT iterator destination
  while (1) {
    pthread_mutex_lock(&ITER_MUTEX);  // Lock Iter Mutex
    if (MORE == 1) {  // No more
      pthread_mutex_unlock(&ITER_MUTEX);  // Unlock Iter Mutex
      return (void *)records;  // Increment with value returned by IndexAFile_MT // Do not call HasMore function if we know there are no more
    }
    if (HTIteratorHasMore(iter) == 0) {
      MORE = 1;  // No more after this, but continue
      HTIteratorGet(iter, &kvp);
      char *file_name = kvp.value;
      pthread_mutex_unlock(&ITER_MUTEX);
      FILE *cfPtr;

      if ((cfPtr = fopen(file_name, "r")) == NULL) {
        printf("File could not be opened\n");
        return 0;
      } else {
        char buffer[BUFFER_SIZE];
        int num_records = 0;

        while (fgets(buffer, BUFFER_SIZE, cfPtr) != NULL) {
          Movie *movie = CreateMovieFromRow(buffer);
          pthread_mutex_lock(&INDEX_MUTEX);  // Lock Index Mutex
          int result = AddMovieTitleToIndex(movieIndex, movie, kvp.key, num_records);
          if (result < 0) {
            fprintf(stderr, "Didn't add MovieToIndex.\n");
          }
          pthread_mutex_unlock(&INDEX_MUTEX);  // Unlock Index Mutex
          num_records++;
          (*records)++;  // Increment Movie count
          DestroyMovie(movie);  // Done with this now
        }
        fclose(cfPtr);
      }
      return (void *)records;
    }
    HTIteratorGet(iter, &kvp);
    HTIteratorNext(iter);
    pthread_mutex_unlock(&ITER_MUTEX);  // Unlock Iter Mutex
    char *file_name = kvp.value;
    FILE *cfPtr;

    if ((cfPtr = fopen(file_name, "r")) == NULL) {
      printf("File could not be opened\n");
      return 0;
    } else {
      char buffer[BUFFER_SIZE];
      int num_records = 0;

      while (fgets(buffer, BUFFER_SIZE, cfPtr) != NULL) {
        Movie *movie = CreateMovieFromRow(buffer);
        pthread_mutex_lock(&INDEX_MUTEX);  // Lock Index Mutex
        int result = AddMovieTitleToIndex(movieIndex, movie, kvp.key, num_records);
        if (result < 0) {
          fprintf(stderr, "Didn't add MovieToIndex.\n");
        }
        pthread_mutex_unlock(&INDEX_MUTEX);  // Unlock Index Mutex
        num_records++;
        (*records)++;  // Increment Movie count
        DestroyMovie(movie);  // Done with this now
      }
      fclose(cfPtr);
    }
  }
  return (void*)records;
}
