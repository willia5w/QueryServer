// Modified by Dan Williams 7/31/2019

/*
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

#include "DirectoryParser.h"
#include "Movie.h"
#include "DocIdMap.h"
#include "Assert007.h"

#define kBufferSize 1000
/**
 * \fn Parses the files that are in a provided DocIdMap.
 *
 * Given a map of all the files that we want to index
 * and search, open each file and index the contents to index
 *
 * \param docs the DocIdMap that contains all the files we want to parse.
 * \param the index to hold all the indexed docs.
 *
 * \return The number of records parsed
 */
int ParseTheFiles(DocIdMap docs, MovieTitleIndex index) {
  // STEP 6: Implement ParseTheFiles.
  // Iterate through all the docs, and add to the index,
  // utilizing IndexTheFile.
  DocIdIter iter = CreateDocIdIterator(docs);
  
  while (iter.HasNext(doc)) {
    iter = iter.Next();
    // Get DocId as PayLoad
  }
  DestroyDocIdIterator(iter);
  
  char* file = GetFileFromId(docs, docId);
  
  return IndexTheFile(char *file, uint64_t doc_id, index); 
}


// Returns the number of records indexed
int IndexTheFile(char *file, uint64_t doc_id, MovieTitleIndex index) {
  FILE *cfPtr;

  if ((cfPtr = fopen(file, "r")) == NULL) {
    printf("File could not be opened\n");
    return 0;
  } else {
    char buffer[kBufferSize];
    int row = 0;

    while (fgets(buffer, kBufferSize, cfPtr) != NULL) {
      Movie *movie = CreateMovieFromRow(buffer);
      int result = AddMovieTitleToIndex(index, movie, doc_id, row);
      if (result < 0) {
        fprintf(stderr, "Didn't add MovieToIndex.\n");
      }
      row++;
      DestroyMovie(movie);  // Done with this now
    }
    fclose(cfPtr);
    return row;
  }
}

