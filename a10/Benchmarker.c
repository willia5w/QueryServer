/*
 * Modified by Dan Williams 8/9/2019
 * Added print statements and additional test for single thread
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/time.h>
#include <time.h>

#include <unistd.h>
#include <errno.h>

#include "Hashtable.h"

#include "DocIdMap.h"
#include "FileCrawler.h"
#include "MovieTitleIndex.h"

#include "DirectoryParser.h"
#include "DirectoryParser_MT.h"


int main(int argc, char *argv[]) {
  // Check arguments
  if (argc != 2) {
    printf("Wrong number of arguments.\n");
    printf("usage: main <directory_to_crawl>\n");
    return 0;
  }

  DocIdMap docs;
  
  // Create a DocIdMap
  docs = CreateDocIdMap();
  CrawlFilesToMap(argv[1], docs);
  printf("Crawled %d files.\n", NumElemsInHashtable(docs));
  printf("Created DocIdMap\n");

  // You can use these to do timing from Benchmarker if you want.
  // clock_t start2, end2;
  // double cpu_time_used;

  // Create the index

  // ======================
  // Index using no threads
  // ======================
  MovieTitleIndex doc_index = CreateMovieTitleIndex();
  printf("Using no threads...\n");
  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, doc_index);
  printf("%d entries in the index.\n", NumElemsInHashtable(doc_index->ht));
  DestroyMovieTitleIndex(doc_index);

  // ======================
  // Index using 1 thread
  // ======================
  printf("Using 1 thread...\n");
  MovieTitleIndex  doc_index_MT = CreateMovieTitleIndex();
  printf("Parsing and indexing files...\n");
  ParseTheFiles_MT(docs, doc_index_MT, 1);
  printf("%d entries in the index.\n", NumElemsInHashtable(doc_index_MT->ht));
  DestroyMovieTitleIndex(doc_index_MT);

  // ======================
  // Index using 5 threads
  // ======================
  printf("Using 5 threads...\n");
  doc_index_MT = CreateMovieTitleIndex();
  printf("Parsing and indexing files...\n");
  ParseTheFiles_MT(docs, doc_index_MT, 5);
  printf("%d entries in the index.\n", NumElemsInHashtable(doc_index_MT->ht));
  DestroyMovieTitleIndex(doc_index_MT);

  // ======================
  // Index using 10 threads
  // ======================
  printf("Using 10 threads...\n");
  doc_index_MT = CreateMovieTitleIndex();
  printf("Parsing and indexing files...\n");
  ParseTheFiles_MT(docs, doc_index_MT, 10);
  printf("%d entries in the index.\n", NumElemsInHashtable(doc_index_MT->ht));
  DestroyMovieTitleIndex(doc_index_MT);

  // Add more tests (or modify the above) if you'd like

  DestroyDocIdMap(docs);
}

