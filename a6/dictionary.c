#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"



TrieNode* CreateTrieNode(char letter) {

}

void DestroyTrieNode(TrieNode* node) {

}

int AddWordTrie(DictTrie *trie, char* word) {
 
}

int ContainsWordTrie(DictTrie *trie, char* word) {

}

DictTrie* LoadDictionaryTrie(char *filename) {

}
DictArray* LoadDictionaryArray(char *filename) {

}

int AddWordArray(DictArray *dict, char* word) {

}

int ContainsWordArray(DictArray *dict, char* word) {

}
  
DictArray* CreateDictArray() {

}

void DestroyDictArray(DictArray* dict) {

}

DictTrie* CreateDictTrie() {

}

void DestroyDictTrie(DictTrie* trie) {

}

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

