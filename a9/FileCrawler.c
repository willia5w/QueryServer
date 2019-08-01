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
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "FileCrawler.h"
#include "DocIdMap.h"
#include "LinkedList.h"


void CrawlFilesToMap(const char *dir, DocIdMap map) {
  // STEP 3: Implement the file crawler.
  // Use namelist (declared below) to find files and put in map.
  // NOTE: There may be nested folders.
  // Be sure to lookup how scandir works. Don't forget about memory use.
  struct stat s;
  struct dirent **namelist;

  if (stat(dir, &s) < 0) { // Run stat
    printf("FAIL!");
  }
  if (S_ISDIR(s.st_mode) == 0) {  // Base case, file ready for Put
    PutFileInMap(dir, map);
    return;
  } else if (S_ISDIR(s.st_mode) != 0) {  // Case: Is Directory
    int n;
    n = scandir(dir, &namelist, 0, alphasort);  // Run scandir()
    for (int i = 2; i < n; i++) { // Start index 2 to skip ".."
      struct stat st;  // Instantiate new stat
      char* temp_file_name = malloc(sizeof(char) * 100);
      snprintf(temp_file_name, strlen(dir) + strlen(namelist[i]->d_name) + 2,
      "%s%s", dir, namelist[i]->d_name);
      stat(temp_file_name, &st); // Run stat for st_mode tests

      if (S_ISDIR(st.st_mode) != 0) {  // Case: Directory
        char file_name[100];
        strcpy(file_name, temp_file_name);
        strcat(file_name, "/");  //Add "/" to end of dir
        free(temp_file_name);
        CrawlFilesToMap(file_name, map);
      } else if (S_ISDIR(st.st_mode) == 0) {
        CrawlFilesToMap(temp_file_name, map);
      }
    }
    for (int i = 0; i < n; i++) {
      free(namelist[i]);
    }
    free(namelist);
  }
}

