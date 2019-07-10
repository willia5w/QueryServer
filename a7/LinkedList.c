// CS 5007, Northeastern University, Seattle
// Summer 2019
// Adrienne Slaughter
//
// Inspired by UW CSE 333; used with permission.
//
// This is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published
//  by the Free Software Foundation, either version 3 of the License,
//  or (at your option) any later version.
// It is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.

#include "LinkedList.h"
#include "LinkedList_priv.h"
#include "Assert007.h"

#include <stdio.h>
#include <stdlib.h>

LinkedList CreateLinkedList() {
  // New Linked List is a pointer to the ll_head struct
  LinkedList list = (LinkedList)malloc(sizeof(LinkedListHead));
  if (list == NULL) {
    // out of memory
    return (LinkedList) NULL;
  }

  // Step 1.
  // initialize the newly allocated record structure
  list->num_elements = 0;
  list->head = NULL;
  list->tail = NULL;
  return list;
}

int DestroyLinkedList(LinkedList list,
    LLPayloadFreeFnPtr payload_free_function) {
    if (list == NULL) {  // List destroy failure
        return 1;
    }

    Assert007(list != NULL);
    Assert007(payload_free_function != NULL);

    // Step 2.
    // Free the payloads, as well as the nodes
    if (list->num_elements == 1) {
      LinkedListNode* only_node = list->head;
      payload_free_function(only_node->payload);
      DestroyLinkedListNode(only_node);  // Pointing to same node as tail
      list->num_elements--;  // Now 0
      free(list);  // List empty
      return 0;  // List destroy success
    } else {
      while (list->num_elements != 0) {
        LinkedListNode* current_node = list->head;  // Copy head
        list->head = list->head->next;  // Dereference head
        payload_free_function(current_node->payload);  // Free payload
        DestroyLinkedListNode(current_node);  // Free current node
        list->num_elements--;  // Decrement number of nodes
      }
      free(list);  // List empty
      return 0;  // List destroy success
    }
}

unsigned int NumElementsInLinkedList(LinkedList list) {
  Assert007(list != NULL);
  return list->num_elements;
}

LinkedListNodePtr CreateLinkedListNode(void *data) {
    LinkedListNodePtr node = (LinkedListNodePtr)malloc(sizeof(LinkedListNode));
    if (node == NULL) {
        // Out of memory
        return NULL;
    }
    node->payload = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

int DestroyLinkedListNode(LinkedListNode *node) {
  Assert007(node != NULL);
  node->payload = NULL;
  node->next = NULL;
  node->prev = NULL;
  free(node);
  return 0;
}

int InsertLinkedList(LinkedList list, void *data) {
  Assert007(list != NULL);
  Assert007(data != NULL);
  LinkedListNodePtr new_node = CreateLinkedListNode(data);

  if (new_node == NULL) {  // Insertion failure
    return 1;
  }

  if (list->num_elements == 0) {
    Assert007(list->head == NULL);  // debugging aid
    Assert007(list->tail == NULL);  // debugging aid
    list->head = new_node;
    list->tail = new_node;
    new_node->next = new_node->prev = NULL;
    list->num_elements = 1U;
    return 0;
  } else {
    // Step 3. Add Node to head of Linked List
    // typical case; list has >=1 elements
    new_node->next = list->head;  // Update next for new node
    new_node->next->prev = new_node;  // Update prev of previous head
    list->head = new_node;  // Update head of list
    list->num_elements++;  // Increase count of nodes in list
    return 0;
  }
}

int AppendLinkedList(LinkedList list, void *data) {
  LinkedListNode* new_node = CreateLinkedListNode(data);

  if (list == NULL) {  // Append failure
    return -1;
  }
  Assert007(list != NULL);

  // Step 5: implement AppendLinkedList.  It's kind of like
  // InsertLinkedList, but add to the end instead of the beginning.
  if (list->num_elements == 0) {
    Assert007(list->head == NULL);  // debugging aid
    Assert007(list->tail == NULL);  // debugging aid
    list->head = new_node;
    list->tail = new_node;
    new_node->next = new_node->prev = NULL;
    list->num_elements = 1U;
    return 0;
  } else {
    new_node->prev = list->tail;  // Update prev for new node
    list->tail = new_node;  // Update tail of list
    new_node->prev->next = new_node;  // Update next of previous tail
    list->num_elements++;  // Increase count of nodes in list
    return 0;
  }
}

// Copies the pointer to the payload at the head of a list
// to the provided dataPtr, and removes the head node from the list.
//
// INPUT: A pointer to the LinkedList.
// INPUT: A pointer to a pointer that will be updated
//        with the payload at the head of the list. .
//
// Returns 0 if the pop was successful; non-zero for failure.
int PopLinkedList(LinkedList list, void **data) {
    Assert007(list != NULL);
    Assert007(data != NULL);

  // Step 4: implement PopLinkedList.  Make sure you test for
  // and empty list and fail.  If the list is non-empty, there
  // are two cases to consider:
  //
  // (a) a list with a single element in it
  //
  // (b) the general case of a list with >=2 elements in it.
  //
  // Be sure to call free() to deallocate the memory that was
  // previously allocated by InsertLinkedList().

  if (list->num_elements == 0) {  // No elements, FAIL
      return -1;

  } else if (list->num_elements == 1) {  // Head only, Tail already NULL
      LinkedListNode* only_node = list->head;  // De-reference
      *(data) = list->head->payload;  // Copy pointer to payload
      DestroyLinkedListNode(only_node);  // Free head
      list->num_elements--;  // Decrease count of nodes in list
      return 0;

  } else {  // Multiple elements
      LinkedListNode* current_node = list->head;  // De-reference
      *(data) = list->head->payload;  // Copy pointer to payload
      list->head = list->head->next;  // Update head->next pointer
      list->head->prev = NULL;  // Update head->prev pointer to NULL
      DestroyLinkedListNode(current_node);  // Destroy old head
      list->num_elements--;  // Decrease count of nodes in list
      return 0;
  }
}

int SliceLinkedList(LinkedList list, void **data) {
  Assert007(list != NULL);
  Assert007(data != NULL);

  // Step 6: implement SliceLinkedList.
  if (list->num_elements == 0) {  // No elements, FAIL
    return -1;

  } else if (list->num_elements == 1) {  // Head only, Tail already NULL
    LinkedListNode* only_node = list->tail;  // De-reference
    *(data) = list->tail->payload;  // Copy pointer to payload
    DestroyLinkedListNode(only_node);  // Free tail
    list->num_elements--;  // Decrease count of nodes in list
    return 0;

  } else {  // Multiple elements
    LinkedListNode* old_tail = list->tail;  // De-reference
    *(data) = list->tail->payload;  // Copy pointer to payload
    list->tail = list->tail->prev;  // Update tail
    DestroyLinkedListNode(old_tail);  // Destroy old tail
    list->num_elements--;  // Decrease count of nodes in list
    return 0;
  }
}

void SortLinkedList(LinkedList list,
                    unsigned int ascending,
                    LLPayloadComparatorFnPtr compare) {
    Assert007(list != NULL);
    if (list->num_elements <2) {
        return;
    }

    int swapped;
    do {
      LinkedListNodePtr curnode = list->head;
      swapped = 0;

      while (curnode->next != NULL) {
        // compare this node with the next; swap if needed
        int compare_result = compare(curnode->payload,
            curnode->next->payload);

        if (ascending) {
          compare_result *= -1;
        }

        if (compare_result < 0) {
          // swap
          void* tmp;
          tmp = curnode->payload;
          curnode->payload = curnode->next->payload;
          curnode->next->payload = tmp;
          swapped = 1;
        }
        curnode = curnode->next;
      }
    } while (swapped);
}

void PrintLinkedList(LinkedList list) {
    printf("List has %lu elements. \n", list->num_elements);
}


LLIter CreateLLIter(LinkedList list) {
  Assert007(list != NULL);
  Assert007(list->num_elements > 0);

  LLIter iter = (LLIter)malloc(sizeof(struct ll_iter));
  Assert007(iter != NULL);

  iter->list = list;
  iter->cur_node = list->head;
  return iter;
}

int LLIterHasNext(LLIter iter) {
  Assert007(iter != NULL);
  return (iter->cur_node->next != NULL);
}

int LLIterNext(LLIter iter) {
  Assert007(iter != NULL);  // Linked List does not exist

  // Step 7: if there is another node beyond the iterator, advance to it,
  // and return 0. If there isn't another node, return 1.
  if (LLIterHasNext(iter) == 1) {
      iter->cur_node = iter->cur_node->next;  // Iterate to next Node
      return 0;
  } else {
      return 1;  // Already on last node
  }
}

int LLIterGetPayload(LLIter iter, void** data) {
  Assert007(iter != NULL);
  *data = iter->cur_node->payload;
  return 0;
}


int LLIterHasPrev(LLIter iter) {
  Assert007(iter != NULL);
  return (iter->cur_node->prev != NULL);
}

int LLIterPrev(LLIter iter) {
  Assert007(iter != NULL);
  // Step 8:  if there is another node beyond the iterator, go to it,
  // and return 0. If not return 1.
  if (LLIterHasPrev(iter)) {
    iter->cur_node = iter->cur_node->prev;
    return 0;  // Another node is present, can iterate further
  }
  return -1;  // Cannot iterate any further
}

int DestroyLLIter(LLIter iter) {
  Assert007(iter != NULL);
  iter->cur_node = NULL;
  iter->list = NULL;
  free(iter);
  return 0;
}

int LLIterInsertBefore(LLIter iter, void* payload) {
  Assert007(iter != NULL);
  if ((iter->list->num_elements <= 1) ||
      (iter->cur_node == iter->list->head)) {
    // insert item
    return InsertLinkedList(iter->list, payload);
  }

  LinkedListNodePtr new_node = CreateLinkedListNode(payload);
  if (new_node == NULL) return 1;

  new_node->next = iter->cur_node;
  new_node->prev = iter->cur_node->prev;
  iter->cur_node->prev->next = new_node;
  iter->cur_node->prev = new_node;

  iter->list->num_elements++;

  return 0;
}

int LLIterDelete(LLIter iter, LLPayloadFreeFnPtr payload_free_function) {
  Assert007(iter != NULL);

  // Step 9: implement LLIterDelete. There are several cases
  // to consider:
  //
  // - Case 1: the list becomes empty after deleting.
  if (iter->list->num_elements == 1) {
    LinkedListNode* only_node = iter->cur_node;  // De-reference
    payload_free_function(only_node->payload);  // Free payload
    DestroyLinkedListNode(only_node);  // Free node
    DestroyLLIter(iter);  // Iter no longer needed, delete
    iter->list->num_elements--;  // Decrement node count
    return 1;
  } else if (!LLIterHasPrev(iter)) {  // - Case 2: iter points at head
    LinkedListNode* old_head = iter->cur_node;  // De-reference
    iter->list->head = iter->cur_node->next;  // Update head
    LLIterNext(iter);  // Iterate next
    iter->cur_node->prev = NULL;  // Update prev pointer of first node
    payload_free_function(old_head->payload);  // Free payload
    DestroyLinkedListNode(old_head);  // Free node
    iter->list->num_elements--;  // Decrement node count
    return 0;
  } else if (!LLIterHasNext(iter)) {  // - Case 3: iter points at tail
    LinkedListNode* old_tail = iter->cur_node;  // De-reference
    iter->list->tail = iter->cur_node->prev;  // Update tail
    LLIterPrev(iter);  // Iterate prev
    iter->cur_node->next = NULL;  // Update next pointer of last node
    payload_free_function(old_tail->payload);  // Free payload
    DestroyLinkedListNode(old_tail);  // Free node
    iter->list->num_elements--;  // Decrement node count
    return 0;
  } else {  // - Splice case: iter points in the middle of a list
    LinkedListNode* middle_node = iter->cur_node;  // De-reference
    iter->cur_node->prev->next = iter->cur_node->next;  // Re-assign next
    iter->cur_node->next->prev = iter->cur_node->prev;  // Re-assign prev
    LLIterNext(iter);  // Iterate next
    payload_free_function(middle_node->payload);  // Free payload
    DestroyLinkedListNode(middle_node);  // Free node
    iter->list->num_elements--;  // Decrement node count
    return 0;
  }
}

