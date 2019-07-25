// Modified by Dan Williams 7/15/2019

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>  // for bool type (true, false)

#include "Hashtable.h"
#include "Hashtable_priv.h"
#include "LinkedList.h"
#include "Assert007.h"

// a free function that does nothing
static void NullFree(void *freeme) { }

static void FreeKVP(void *freeme) {
  free(freeme);
}

Hashtable CreateHashtable(int num_buckets) {
  if (num_buckets == 0)
    return NULL;

  Hashtable ht = (Hashtable)malloc(sizeof(struct hashtableInfo));

  if (ht == NULL) {
    return NULL;
  }

  ht->num_buckets = num_buckets;
  ht->num_elements = 0;
  ht->buckets =
      (LinkedList*)malloc(num_buckets * sizeof(LinkedList));

  if (ht->buckets == NULL) {
    free(ht);
    return NULL;
  }

  for (int i=  0; i < num_buckets; i++) {
    ht->buckets[i] = CreateLinkedList();
    if (ht->buckets[i] == NULL) {
      // Need to free everything and then return NULL
      for (int j = 0; j < i; j++) {
        DestroyLinkedList(ht->buckets[j], NULL);
      }
      free(ht);
      return NULL;
    }
  }
  return ht;
}

void DestroyHashtable(Hashtable ht, ValueFreeFnPtr valueFreeFunction) {
  // Go through each bucket, freeing each bucket
  for (int i = 0; i < ht->num_buckets; i++) {
    LinkedList bucketlist = ht->buckets[i];
    HTKeyValuePtr nextKV;

    // Free the values in the list; then free the list
    if (NumElementsInLinkedList(bucketlist) > 0) {
      LLIter list_iter = CreateLLIter(bucketlist);

      LLIterGetPayload(list_iter, (void**)&nextKV);
      valueFreeFunction(nextKV->value);

      // Now loop through the rest
      while (LLIterHasNext(list_iter) == 1) {
        LLIterNext(list_iter);
        LLIterGetPayload(list_iter, (void**)&nextKV);
        valueFreeFunction(nextKV->value);
      }
      DestroyLLIter(list_iter);
    }
    DestroyLinkedList(bucketlist, FreeKVP);
  }

  // free the bucket array within the table record,
  // then free the table record itself.
  free(ht->buckets);
  free(ht);
}
// LinkedList Iterator to search bucket for key
// Returns 0 if Key found in chain
// Returns -1 never found
int KeyInChain(LLIter lliter, uint64_t key, HTKeyValue **kvp) {
  Assert007(lliter != NULL);

  while (1) {  // While bucket remains iterable
    // Node has key, success
    LLIterGetPayload(lliter, (void **) kvp);
    if ((*kvp)->key == key) {
      return 0;
    }
    // Cannot iterate further, no nodes contain key
    if (LLIterHasNext(lliter) == 0) {
      break;
    }
    LLIterNext(lliter);
  }
  // No nodes contained key, fail
  return -1;
}


// Looks up the given key in the hashtable.
// Returns the Key-Value pair in the provided result pointer.
//
// INPUT:
//   ht: the hashtable to lookup in
//   key: the key to lookup
//   result: If the key is in the hashtable, this is used
//      to return a pointer to a HTKeyValue to the caller.
//      Note, this HTKeyValue is still in the hashtable, so can't
//      be used to free the value.
//
// Returns 0 if the lookup was successful (and the result is valid).
// Returns -1 if the key was not found.
int LookupInHashtable(Hashtable ht, uint64_t key, HTKeyValue *old_key_value) {
  Assert007(ht != NULL);

  // STEP 2: Implement lookup
  int lookup_bucket;
  LinkedList lookup_chain;

  lookup_bucket = HashKeyToBucketNum(ht, key);
  lookup_chain = ht->buckets[lookup_bucket];

  if (NumElementsInLinkedList(lookup_chain) == 0)
    return -1;  // No keys to be found

  LLIter lliter = CreateLLIter(lookup_chain);

  if (lliter == NULL) {
    return -1;  // Iter fail
  }

  HTKeyValue *temp_payload;  // kvp
  if (KeyInChain(lliter, key, &temp_payload) == 0) {
    old_key_value->key = temp_payload->key;
    old_key_value->value = temp_payload->value;
    DestroyLLIter(lliter);
    // free(temp_payload);
    return 0; // Success
  }
  // free(temp_payload);
  return -1;  // Not found, fail
}
// Puts the given key value pair int the hashtable.
//
// INPUT:
//   ht: the hashtable to insert into
//   kvp: the key-value pair to put into the hashtable.
//   old_kvp: if there is a collision, sets the old_kvp to point at the previous value.
//
// Returns 0 if put is successful.
// Returns 1 on failure (e.g., no more memory)
// Returns 2 if the key is already in the hashtable
int PutInHashtable(Hashtable ht,
                   HTKeyValue kvp,
                   HTKeyValue *old_key_value) {
  Assert007(ht != NULL);

  int insert_bucket;
  LinkedList insert_chain;

  ResizeHashtable(ht);

  // calculate which bucket we're inserting into,
  // get the list
  insert_bucket = HashKeyToBucketNum(ht, kvp.key);
  insert_chain = ht->buckets[insert_bucket];

  // STEP 1: Finish the implementation of the put.
  // This is a fairly complex task, so you might decide you want
  // to define/implement a helper function that helps you find
  // and optionally remove a key within a chain, rather than putting
  // all that logic inside here. You might also find that your helper(s)
  // can be reused in step 2 and 3.

  HTKeyValue *new_payload = (HTKeyValue *) malloc(sizeof(HTKeyValue));

  if (new_payload == NULL) {
    return 1;  // New payload fail
  }

  // Copy kpv to new payload
  new_payload->key = kvp.key;
  new_payload->value = kvp.value;

  // Case where new key is first in bucket
  if (NumElementsInLinkedList(insert_chain) == 0) {
    if (InsertLinkedList(insert_chain, (void *) new_payload) != 0) {
      return 1;  // Insert fail
    }
    ht->num_elements += 1;
    return 0;  // Added successfully
  }

  //if (LookupInHashtable(ht, new_payload->key, old_key_value) == 0) {
  if (LookupInHashtable(ht, new_payload->key, old_key_value) == 0) {
    return 2;  // Case where Key exists in bucket
  }

  if (InsertLinkedList(insert_chain, (void *) new_payload) != 0) {
    return 1;  // Insert fail
  }
  ht->num_elements += 1;
  return 0;  // Put success
}

int HashKeyToBucketNum(Hashtable ht, uint64_t key) {
  return key % ht->num_buckets;
}

int NumElemsInHashtable(Hashtable ht) {
  int res = 0;
  for (int i=0; i<ht->num_buckets; i++) {
    res += NumElementsInLinkedList(ht->buckets[i]);
  }
  return res;
}

// Removes the HTKeyValue with the given key from the
//  hashtable.
//
// INPUT:
//   ht: the hashtable
//   key: the key to remove
//   junkKVP: a pointer to the removed HTKeyValue, to
//     allow the value to be freed by the caller.
//
// Returns 0 if the key was found successfully (and junkKVP is valid)
// Returns -1 if the key was not found in the hashtable.
int RemoveFromHashtable(Hashtable ht, uint64_t key, HTKeyValuePtr junkKVP) {

  // STEP 3: Implement Remove
  Assert007(ht != NULL);

  int remove_bucket;
  LinkedList remove_chain;

  remove_bucket = HashKeyToBucketNum(ht, key);
  remove_chain = ht->buckets[remove_bucket];

  if (NumElementsInLinkedList(remove_chain) == 0) {
    return -1;  // Nothing to remove
  }

  LLIter lliter = CreateLLIter(remove_chain);
  if (lliter == NULL) {  // Iterator create fail
    return -1;
  }

  HTKeyValue* temp_payload;  // Temp variable to copy kvp

  if (KeyInChain(lliter, key, &temp_payload)) {
    junkKVP->key = temp_payload->key;
    junkKVP->value = temp_payload->value;

    free(temp_payload);
    LLIterDelete(lliter, &NullFree);  // Delete node iterator is pointing to
    DestroyLLIter(lliter);  // Free iterator

    ht->num_elements--;
    return 0;  // Remove success, key found
  }
  DestroyLLIter(lliter);  // Free iterator
  return -1;  // Key not found
}


uint64_t FNVHash64(unsigned char *buffer, unsigned int len) {
  // This code is adapted from code by Landon Curt Noll
  // and Bonelli Nicola:
  //
  // http://code.google.com/p/nicola-bonelli-repo/
  static const uint64_t FNV1_64_INIT = 0xcbf29ce484222325ULL;
  static const uint64_t FNV_64_PRIME = 0x100000001b3ULL;
  unsigned char *bp = (unsigned char *) buffer;
  unsigned char *be = bp + len;
  uint64_t hval = FNV1_64_INIT;
  /*
   * FNV-1a hash each octet of the buffer
   */
  while (bp < be) {
    /* xor the bottom with the current octet */
    hval ^= (uint64_t) * bp++;
    /* multiply by the 64 bit FNV magic prime mod 2^64 */
    hval *= FNV_64_PRIME;
  }
  /* return our new hash value */
  return hval;
}


uint64_t FNVHashInt64(uint64_t makehash) {
  unsigned char buf[8];
  int i;
  for (i = 0; i < 8; i++) {
    buf[i] = (unsigned char) (makehash & 0x00000000000000FFULL);
    makehash >>= 8;
  }
  return FNVHash64(buf, 8);
}


void ResizeHashtable(Hashtable ht) {
  Assert007(ht != NULL);

  // Resize if the load factor is > 3.
  if (ht->num_elements < 3 * ht->num_buckets)
    return;

  // This is the resize case.  Allocate a new hashtable,
  // iterate over the old hashtable, do the surgery on
  // the old hashtable record and free up the new hashtable
  // record.
  Hashtable newht = CreateHashtable(ht->num_buckets * 9);
  // Give up if out of memory.
  if (newht == NULL)
    return;

  // Loop through the old ht with an iterator,
  // inserting into the new HT.
  HTIter it = CreateHashtableIterator(ht);
  if (it == NULL) {
    // Give up if out of memory.
    DestroyHashtable(newht, &NullFree);
    return;
  }

  HTKeyValue item;
  HTIteratorGet(it, &item);
  HTKeyValue old_kv;

  if (PutInHashtable(newht, item, &old_kv) == 1) {
    // failure, free up everything, return.
    DestroyHashtableIterator(it);
    DestroyHashtable(newht, &NullFree);
    return;
  }

  while (HTIteratorHasMore(it) != 0) {
    HTIteratorNext(it);

    HTKeyValue item;
    HTIteratorGet(it, &item);
    HTKeyValue old_kv;

    if (PutInHashtable(newht, item, &old_kv) == 1) {
      // failure, free up everything, return.
      DestroyHashtableIterator(it);
      DestroyHashtable(newht, &NullFree);
      return;
    }
  }
  // Worked!  Free the iterator.
  DestroyHashtableIterator(it);
  // Sneaky: swap the structures, then free the new table,
  // and we're done.
  {
    struct hashtableInfo tmp;
    tmp = *ht;
    *ht = *newht;
    *newht = tmp;
    DestroyHashtable(newht, &NullFree);
  }
  return;
}


// ==========================
// Hashtable Iterator
// ==========================

// Returns NULL on failure, non-NULL on success.
HTIter CreateHashtableIterator(Hashtable table) {
  if (NumElemsInHashtable(table) == 0) {
    return NULL;
  }
  HTIter iter = (HTIter)malloc(sizeof(HTIterRecord));
  if (iter == NULL) {
    return NULL;  // Couldn't malloc
  }
  iter->ht = table;
  iter->which_bucket = 0;

  while (NumElementsInLinkedList(iter->ht->buckets[iter->which_bucket]) == 0) {
    iter->which_bucket++;
  }
  iter->bucket_iter = CreateLLIter(iter->ht->buckets[iter->which_bucket]);

  return iter;
}


void DestroyHashtableIterator(HTIter iter) {
  // Destroy the list iter
  free(iter->bucket_iter);
  iter->ht = NULL;
  free(iter);
}

// Moves to the next element; does not return.
int HTIteratorNext(HTIter iter) {
  // Step 4: Implement HTIteratorNext
  if (!HTIteratorHasMore(iter))  // Cannot iterate
    return 0;

  if (LLIterHasNext(iter->bucket_iter)) {
    LLIterNext(iter->bucket_iter);  // Iterate success
    return 1;
  }

  if (iter->which_bucket == (iter->ht->num_buckets) - 1) {
    return 0;  // Cannot iterate
  }

  uint64_t i;
  for (i = iter->which_bucket  + 1; i < iter->ht->num_buckets; i++) {
    if (NumElementsInLinkedList(iter->ht->buckets[i]) > 0) {
      iter->which_bucket = i;
      break;
    }
  }

  if (i == iter->ht->num_buckets) {
    return 0;
  }
  return 0;
}


//  0 if there are no more elements.
int HTIteratorHasMore(HTIter iter) {
  if (iter->bucket_iter == NULL) {
    return 0;
  }

  if (LLIterHasNext(iter->bucket_iter) == 1)
    return 1;

  // No more in this iter; are there more buckets?
  int i = iter->which_bucket + 1;
  while (i < (iter->ht->num_buckets)) {
    // Make sure one of them has elements in it
    if ((iter->ht->buckets[i] != NULL) &&
        (NumElementsInLinkedList(iter->ht->buckets[i]) > 0)) {
      return 1;
    }
    i++;
  }
  return 0;
}


int HTIteratorGet(HTIter iter, HTKeyValuePtr dest) {
  Assert007(iter != NULL);

  // Step 6 -- implement HTIteratorGet.
  HTKeyValue *temp_payload;
  if (iter->bucket_iter != NULL) {
    LLIterGetPayload(iter->bucket_iter, (void **) &temp_payload);
    dest->key = temp_payload->key;
    dest->value = temp_payload->value;
    return 0;  // Success
  }
  return 1; // Fail
}

