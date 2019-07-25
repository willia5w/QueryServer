// Modified by Dan Williams 7/24/2019

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "htll/LinkedList.h"
#include "MovieIndex.h"
#include "Movie.h"
#include "MovieReport.h"
#include "./FileParser.h"



int main(int argc, char* argv[]){
  LinkedList movie_list  = ReadFile("data/test");

  // TODO: Parse command-line args to index based on the correct field

  // Expected sample row:
  // id       |type |Title1   |Title2   |IsAdult|Year|?|?|Genres
  // tt0003609|movie|Alexandra|Alexandra|0      |1915|-|-|-

    enum {
      All, Comedy, Reality, TV, Sport, Family, Adventure, Thriller}
      mode = Genre;

    // Compare by attribute within movie struct
    if (argc > 3) { //Incorrect number of arguments
      return -1;
    } else if (strcmp(argv[1], "-t") == 0) {
        // Index 1 is Type
        Index index = BuildMovieIndex(movie_list, Type);
    } else if (strcmp(argv[1], "-y") == 0) {
      // Index 5 is Year
      Index index = BuildMovieIndex(movie_list, Year);
    } else {
      // Default is to index on genre. 
      Index index = BuildMovieIndex(movie_list, Genre);
    }

  PrintReport(index);

  DestroyIndex(index);

  return 0;
}

