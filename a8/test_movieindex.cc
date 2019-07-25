// Modified by Dan Williams 7/24/2019

/*
 *  Adrienne Slaughter, Northeastern University.
 *  Spring 2019
 *  CS 5007
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>


#include "gtest/gtest.h"

extern "C" {
  #include "./MovieIndex.h"
  #include "htll/LinkedList.h"
  #include "MovieIndex.h"
  #include "Movie.h"
  #include "MovieReport.h"
  #include <string.h>
}


const char* movie_row_1 = "tt0003609|movie|Alexandra|Alexandra|0|1915|-|-|-";
const char* movie_row_2 = "tt0003620|short|Among the Mourners|Among the Mourners|0|1914|-|-|Comedy,Short";

void DestroyLLMovie(void *payload) {
  DestroyMovie((Movie*)payload);
}

TEST(Movie, CreateFromRow) {
  // Copying the string from a const to an array
  // (to be more similar to the actual use case)
  char row[1000];
  strcpy(row, movie_row_1);
  // Create from a "good" row
  Movie* m1 = CreateMovieFromRow(row);

  ASSERT_EQ(0, strcmp(m1->id, "tt0003609"));
  ASSERT_EQ(0, strcmp(m1->type, "movie"));
  ASSERT_EQ(0, strcmp(m1->title, "Alexandra"));
  ASSERT_EQ(0, m1->isAdult);
  ASSERT_EQ(1915, m1->year);
  ASSERT_EQ(NULL, m1->genres);
  ASSERT_EQ(-1, m1->runtime);


  // TODO: Create from a improper row
  char improper_row[0];
  Movie* m_error = CreateMovieFromRow(improper_row);

  DestroyMovie(m1);

}

TEST(MovieIndex, CreateDestroy) {
  Index index = CreateIndex();

  //  ASSERT_NE(index, NULL);

  DestroyIndex(index);
}


TEST(MovieIndex, AddMovieToIndex) {
  // Copying the string from a const to an array
  // (to be more similar to the actual use case)
  char row[1000];
  strcpy(row, movie_row_1);
  // Take a movie
  MoviePtr m1 = CreateMovieFromRow(row);
  strcpy(row, movie_row_2);
  MoviePtr m2 = CreateMovieFromRow(row);

  // Create Index
  Index index = CreateIndex();

  // Add movie to index
  AddMovieToIndex(index, m1, Type);

  // Check that movie is in index

  // Check size/num elements (should be num of types, not of movies)
  ASSERT_EQ(NumElemsInHashtable(index), 1);

  // Try to insert movie again
  AddMovieToIndex(index, m1, Type);
  // TODO: What's supposed to happen here?
  // Adds movies to index by their Type
  // Write code to check it.

  // Add another movie to the index (same IndexType)
  AddMovieToIndex(index, m2, Type);

  // Add another movie (different IndexType)
  MoviePtr m3 = CreateMovieFromRow(row);
  AddMovieToIndex(index, m3, Year);

  // TODO: Check result is correct.
  ASSERT_EQ(NumElemsInHashtable(index), 3);

  PrintReport(index);

  // Destroy movie index
  DestroyIndex(index);
}


TEST(MovieIndex, GetMovieSet) {
  // TODO: Implement this test.

  // Create a movie index
  Index index = CreateIndex();

  // Add a bunch of movies with different vals for the index
  const char* movie_row_1 = "tt1|movie|Hey Arnold|Nickelodeon|0|1998|-|-|Comedy";
  const char* movie_row_2 = "tt2|short|Pokemon|CN|0|2001|-|-|Action";

  movie_1 = CreateMovieFromRow(movie_row_1);
  movie_2 = CreateMovieFromRow(movie_row_2);

  AddMovieToIndex(index, movie_1, Type);
  AddMovieToIndex(index, movie_2, Type);

  // Get the set
  // Check that the movies are all in the set
  ASSERT_EQ(GetMovieSet(index, movie_row_1), "Hey Arnold");
  ASSERT_EQ(GetMovieSet(index, movie_row_2), "Pokemon");

  // Destroy the movie index
  DestroyIndex(index);

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

