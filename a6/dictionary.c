//
// Created by Daniel Williams on 2019-06-24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"



TrieNode* CreateTrieNode(char letter) {
    TrieNode* trieNode = (TrieNode*)malloc(sizeof(TrieNode));
    if (trieNode == NULL) {
        printf("Unable to malloc trieNode");
        return NULL;
    }
    TrieNode* children[kNumLetters];
    trieNode->my_letter = letter;
    trieNode->is_word = 0;
    return trieNode;
}
/*
void DestroyTrieNode(TrieNode* node) {
    free(node);
}

int AddWordTrie(DictTrie *trie, char* word) {
 
}

int ContainsWordTrie(DictTrie *trie, char* word) {
    int size = strlen(word);
    if (trie->root != NULL) {
        node = trie->root;
        for (int i = 0; i<=size - 1; i++) {
            if (node->my_letter == word[i] && node->is_word == 0) {
                node = node->children;
            }
        }
        if (node->my_letter = word[sizeof(array)/sizeof(array[0]) - 1] && node->is_word == 1){
            return 1;
        } else {
            return 0;
        }

    }
}

DictTrie* LoadDictionaryTrie(char *filename) {

}
DictArray* LoadDictionaryArray(char *filename) {

    FILE *cfPtr;

    if ((cfPtr = fopen(filename, "r")) == NULL) {
        printf("File could not be opened\n");
    } else {
        DictArray* newArray = CreateDictArray();
        char* c;

        while (scanf("%15s", c) != EOF) {
            // Reads file character by character
            c = fgetc(cfPtr);
            AddWordArray(newArray, c);
            newArray->num_words++;
        }
        fclose(cfPtr);
        return newArray;
    }

    //int main(){
     //   ReadFile();
      //  return 0;
    //}
}

int AddWordArray(DictArray *dict, char* word) {
    if (ContainsWordArray(dict, word)) {
        return 2;
    } else if (dict->num_words >= kMaxNumWords) {
        return 0;
    } else {
        char* new = (char*)malloc(sizeof(word + 1)); // 1 may not be necessary
        if (new == NULL) {
            return 0;
        }
        new = word; // May have to build new word here letter by letter
        dict->words[dict->num_words++] = new;
        return 1;
    }
}

// Terminating char!!! '/0'
int ContainsWordArray(DictArray *dict, char* word) {

    int first = 0;
    int last = sizeof(dict->num_words) - 1;
    int middle = (first+last)/2;

    while (first <= last) {
        if (strcmp(dict->words[middle], word) < 0) {
            first = middle + 1;
        } else if (strcmp(dict->words[middle], word) {
            return 1;
        }
        else
            last = middle - 1;

        middle = (first + last)/2;
    }
    if (first > last) {
        return 0;
    }
}
  
DictArray* CreateDictArray() {
    DictArray* dictArray = (DictArray*)malloc(sizeof(dictArray));
    if (dictArray == NULL) {
        printf("Unable to malloc dictArray");
        return NULL;
    }
    char* words[kMaxNumWords];
    int num_words;
    return dictArray;
}

void DestroyDictArray(DictArray* dict) {
    free(dict);
}
*/
DictTrie* CreateDictTrie() {
    DictTrie* dictTrie= (DictTrie*)malloc(sizeof(dictTrie));
    if (dictTrie == NULL) {
        printf("Unable to malloc dictTrie");
        return NULL;
    }
    char rLetter = '*';
    //char rLetter[2] = {'*', '\0'};
    TrieNode* root = CreateTrieNode(rLetter);
    return dictTrie;
}

void DestroyDictTrie(DictTrie* trie) {
    free(trie);
}
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
  PrintReportHelper(trie->root, word, 0); 
}

*/