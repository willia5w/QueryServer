//
// Created by Daniel Williams on 2019-06-24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"



TrieNode* CreateTrieNode(char letter) {
    TrieNode* trieNode = (TrieNode*)malloc(sizeof(TrieNode*));
    if (trieNode == NULL) {
        printf("Unable to malloc trieNode");
        return NULL;
    }
    TrieNode* children[kNumLetters];
<<<<<<< HEAD

    for (int i = 0; i < kNumLetters; i++)
        trieNode->children[i] = NULL;

=======
    for (int i = 0; i < kNumLetters; i++)
        trieNode->children[i] = NULL;
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
    trieNode->my_letter = letter;
    trieNode->is_word = 0;
    return trieNode;
}

void DestroyTrieNode(TrieNode* node) {
    if (node) {
        for (int i = 0; i < kNumLetters; i++) {
            DestroyTrieNode(node->children[i]);
        }
        free(node);
    }
}

int AddWordTrie(DictTrie *trie, char* word) {

<<<<<<< HEAD
    char *new_word;
    new_word = word;
    int word_size = strlen(new_word);
    TrieNode* node = trie->root;

    for (int i = 0; i < word_size; i++) {

        int letter_index = new_word[i]; // Letter cast as int
        int child_index = letter_index - kAlphaStart; // Calculate child node index

        if (i == word_size - 1) {   // Last letter of new word
            if (node->children[child_index] == NULL) { // Letter did not exist here
                node->children[child_index] = CreateTrieNode(new_word[i]);  // Create new node
                node->children[child_index]->is_word = 1; // Set flag
                return 1;  // Add success
            } else if (node->children[child_index]->is_word == 0) { // Letter exists
                node->children[child_index]->is_word = 1; // Mark as ending of new word
                return 1;
            } else { // Word already in trie
                return 2;
            }
        }

        if (node->children[child_index] != NULL) { // Letter exists, not last letter of new word
            node = node->children[child_index]; // Continue to adding next letter
        } else { // New letter to add at this level in the trie
            TrieNode* new_node = CreateTrieNode(new_word[i]);
            node->children[child_index] = new_node; // Create node to add letter to trie
            node = node->children[child_index]; // Reassign current node
        }
=======
//    char* new_word = (char*)malloc(sizeof(word));
//    if (new_word == NULL) {
//        printf("Unable to malloc word");
//        return 0;
//    }
    char* new_word;
    new_word = word;
    int word_size = sizeof(new_word);
    printf("%d", word_size);

    if (word_size > 0) {
        // Add everything up to final letter
        for (int i = 0; i < word_size - 1; i++) {
            int letter_index = new_word[i]; // Letter cast as int
            int child_index = letter_index - kAlphaStart; // Calculate child node index

            if (new_word[i] != trie->root->children[child_index]->my_letter) {
                trie->root->children[child_index]->my_letter = new_word[i];
            }
            // if i = word length - 1, check isWord
        }
    }
    // Final letter of added word or only letter
    int letter_index = new_word[word_size]; // Letter cast as int
    int child_index = letter_index - kAlphaStart; // Calculate child node in

    if (new_word[word_size] != trie->root->children[child_index]->my_letter) {
        // Set letter
        trie->root->children[new_word[word_size] - kAlphaStart]->my_letter = new_word[word_size];
        // Update flag
        trie->root->children[new_word[word_size] - kAlphaStart]->is_word = 1;
    } else if (trie->root->children[new_word[word_size] - kAlphaStart]->is_word == 0){
        // Longer string including this word exists, flag this point as ending of new word
        trie->root->children[new_word[word_size] - kAlphaStart]->is_word = 1;
        return 1;
    } else {
        // Word already in DictTrie
        return 2;
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
    }
    return 0;
}

int ContainsWordTrie(DictTrie *trie, char* word) {
<<<<<<< HEAD

    char* new_word;
    new_word = word;
    int word_size = strlen(word);
    TrieNode* node = trie->root;

    for (int i = 0; i < word_size; i++) {

        int letter_index = new_word[i]; // Letter cast as int
        int child_index = letter_index - kAlphaStart; // Calculate child node index


        if (i == word_size - 1) {   // Last letter of new word
            if (node->children[child_index] == NULL) { // No such word in trie
                return 0;
            } else if (node->children[child_index]->is_word == 0) { // Letter exists, but not word
                return 0;
            } else { // Match found
                return 1;
            }
        }

        if (node->children[child_index] && word[i] == node->children[child_index]->my_letter) {
            node = node->children[child_index];
=======
    int size = strlen(word);
    if (trie->root != NULL) {
        int j = 0;
        TrieNode* node = trie->root->children[j];
        for (int i = 0; i<=size - 1; i++) {
            for (int k = 0; k <= kNumLetters; k++) {
                if (node->my_letter != word[i] && node->is_word == 0) {
                    j++;
                }
            }
        }
        if (node->my_letter == word[sizeof(word)/sizeof(word[0]) - 1] && node->is_word == 1){
            return 1;
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
        } else {
            return 0;
        }
    }
    return 0;
}
/*
DictTrie* LoadDictionaryTrie(char *filename) {

    DictTrie* trie = CreateDictTrie();

    char c[kMaxWordLen];

    FILE *cfPtr;
    if ((cfPtr = fopen(filename, "r")) == NULL) {
        printf("File could not be opened\n");
    }

    while (fscanf(cfPtr,"%s", c) == 1) {
        AddWordTrie(trie, c);
    }
    fclose(cfPtr);
    return trie;
}
<<<<<<< HEAD

=======
 */
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
DictArray* LoadDictionaryArray(char *filename) {
    DictArray* dict = CreateDictArray();

<<<<<<< HEAD
    DictArray* arr = CreateDictArray();

=======
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
    char c[kMaxWordLen];

    FILE *cfPtr;
    if ((cfPtr = fopen(filename, "r")) == NULL) {
        printf("File could not be opened\n");
    }

    while (fscanf(cfPtr,"%s", c) == 1) {
<<<<<<< HEAD
        AddWordArray(arr, c);
    }
    fclose(cfPtr);
    return arr;
=======
        AddWordArray(dict, c);
    }
    fclose(cfPtr); //close dict file
    return dict;
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
}

int AddWordArray(DictArray *dict, char* word) {
    char* new_word = (char*)malloc(sizeof(word));
    if (new_word == NULL) {
        printf("Unable to malloc word");
        return 0;
    }

<<<<<<< HEAD
    for (int i = 0; i < strlen(word); i++) {
=======
    // new_word = strcpy(new_word, word);
    for (int i = 0; i < sizeof(word); i++) {
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
        new_word[i] = word[i];
    }

    if (ContainsWordArray(dict, new_word)) {
        return 2;
    } else {
<<<<<<< HEAD
        dict->words[dict->num_words] = new_word;
        dict->num_words++;
=======
        dict->num_words++;
        dict->words[dict->num_words] = new_word;
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
        return 1;
    }
}

int ContainsWordArray(DictArray *dict, char* word) {
    if (dict->num_words == 0) {
        return 0;
    }

    int first = 0;
<<<<<<< HEAD
    int last = dict->num_words - 1;
=======
    int last = dict->num_words;
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
    int middle;

    while (first <= last) {
        middle = (first + last)/2;
<<<<<<< HEAD
        if (strcmp(dict->words[middle], word) == 0) {
            return 1;
        } else if (strcmp(dict->words[middle], word) > 0) {
            last = middle - 1;
        } else {
=======
        if (strcmp(dict->words[middle], word) == 0) { // Match found
            return 1;
        }
        if (strcmp(dict->words[middle], word) == 1) { // word < current middle
            last = middle - 1;
        }
        if (strcmp(dict->words[middle], word) == -1) {
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
            first = middle + 1;
        }
    }
    return 0;
}

DictArray* CreateDictArray() {
    DictArray* dict_array = (DictArray*)malloc(sizeof(DictArray));
    if (dict_array == NULL) {
        printf("Unable to malloc dictArray");
        return NULL;
    }
    for (int i = 0; i < kMaxNumWords - 1; i++) {
        dict_array->words[i] = NULL;
    }
    dict_array->num_words = 0;
    return dict_array;
}

void DestroyDictArray(DictArray* dict) {
    if (dict && dict->num_words > 0) {
        for (int i = 0; i < dict->num_words; i++) {
            free(dict->words[i]);
        }
        free(dict);
    } else {
        free(dict);
    }
}

DictTrie* CreateDictTrie() {
    DictTrie* dictTrie = (DictTrie*)malloc(sizeof(dictTrie));
    if (dictTrie == NULL) {
        printf("Unable to malloc dictTrie");
        return NULL;
    }
    char rLetter = '*';
    dictTrie->root = CreateTrieNode(rLetter);
    return dictTrie;
}

void DestroyDictTrie(DictTrie* trie) {
<<<<<<< HEAD
    DestroyTrieNode(trie->root);
    free(trie);
}
=======

    DestroyTrieNode(trie->root);
    free(trie);
}

>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
/*

void PrintReportHelper(TrieNode* cur_node, char* cur_word, int level) {
  cur_word[level] = cur_node->my_letter; 
  
  if (cur_node->is_word) {
    cur_word[level + 1] = '\0';
    for (int i = 0; i < level; i++) {
      printf("  ");
    }
    printf("%s\n", cur_word); 
  }
  
  for (int i=0; i<kNumLetters; i++) {
    if (cur_node->children[i] != NULL) {
      PrintReport(cur_node->children[i], cur_word, level + 1); 
    }
  }
}

void PrintReport(TrieNode* cur_node) {
  char word[100] = {0};
  PrintReportHelper(cur_node, word, 0);
}
*/

