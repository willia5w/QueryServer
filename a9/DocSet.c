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

#include "DocSet.h"
#include "Hashtable.h"
#include "Util.h"

/**
 * Adds a reference to a doc/row to the set.
 *
 * \param set The DocumentSet to add the movie to
 * \param doc_id Which document/file the movie is stored in
 * \param row_id Which row in the file the movie can be found.
 *
 * \return 0 if successful.
 */
int AddDocInfoToSet(DocumentSet set,  uint64_t docId, int rowId) {
  // STEP 4: Implement AddDocInfoToSet.
  // Make sure there are no duplicate rows or docIds.

  int* add_rowId[100] = malloc(sizeof(int) * 100);
  add_rowId = rowId;

  if (DocumentSetContainsDoc(set, docId) == 0) {
    free(add_rowId);
    return -1;
  } else {
      HTKeyValue new_doc;
      new_doc.key = docId;  // Update key
      new_doc.value = add_rowId;  // Update value
      HTKeyValue old_kpv; // Provide old kvp

      PutFileInMap(set->desc, set->doc_index); // Add row data to create Movie
      PutInHashtable(set->doc_index, new_doc, old_kpv);
      return 0;
  }
}

/**
 * Determines if a DocumentSet contains movies from a specified
 * document or file.
 *
 * \param set The DocumentSet to query
 * \param doc_id Which doc to look for.
 *
 * \return 0 if the doc_id is found, -1 otherwise.
 */
int DocumentSetContainsDoc(DocumentSet set, uint64_t docId) {
  // STEP 5: Implement DocumentSetContainsDoc
  HTKeyValuePtr result;
  int check = LookupInHashtable(set->doc_index, docId, result);

  if (check == 0) {
    return 0;
  } else {
    return -1;
  }
}

void PrintOffsetList(LinkedList list) {
  printf("Printing offset list\n");
  LLIter iter = CreateLLIter(list);
  int* payload;
  while (LLIterHasNext(iter) != 0) {
    LLIterGetPayload(iter, (void**)&payload);
    printf("%d\t", *((int*)payload));
    LLIterNext(iter);
  }
}


DocumentSet CreateDocumentSet(char *desc) {
  DocumentSet set = (DocumentSet)malloc(sizeof(struct docSet));
  if (set == NULL) {
    // Out of memory
    printf("Couldn't malloc for movieSet %s\n", desc);
    return NULL;
  }
  int len = strlen(desc);
  set->desc = (char*)malloc((len + 1) *  sizeof(char));
  if (set->desc == NULL) {
    printf("Couldn't malloc for movieSet->desc");
    return NULL;
  }
  snprintf(set->desc, len + 1, "%s", desc);
  set->doc_index = CreateHashtable(16);
  return set;
}


void DestroyOffsetList(void *val) {
  LinkedList list = (LinkedList)val;
  DestroyLinkedList(list, &SimpleFree);
}

void DestroyDocumentSet(DocumentSet set) {
  // Free desc
  free(set->desc);
  // Free doc_index
  DestroyHashtable(set->doc_index, &DestroyOffsetList);
  // Free set
  free(set);
}

