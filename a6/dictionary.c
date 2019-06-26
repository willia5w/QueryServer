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
    for (int i = 0; i < kNumLetters; i++)
        trieNode->children[i] = NULL;
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
    }
    return 0;
}

int ContainsWordTrie(DictTrie *trie, char* word) {
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
        } else {
            return 0;
        }

    }
    return 0;
}
/*
DictTrie* LoadDictionaryTrie(char *filename) {

}
 */
DictArray* LoadDictionaryArray(char *filename) {
    DictArray* dict = CreateDictArray();

    char c[kMaxWordLen];

    FILE *cfPtr;
    if ((cfPtr = fopen(filename, "r")) == NULL) {
        printf("File could not be opened\n");
    }

    while (fscanf(cfPtr,"%s", c) == 1) {
        AddWordArray(dict, c);
    }
    fclose(cfPtr); //close dict file
    return dict;
}

int AddWordArray(DictArray *dict, char* word) {
    char* new_word = (char*)malloc(sizeof(word));
    if (new_word == NULL) {
        printf("Unable to malloc word");
        return 0;
    }

    // new_word = strcpy(new_word, word);
    for (int i = 0; i < sizeof(word); i++) {
        new_word[i] = word[i];
    }

    if (ContainsWordArray(dict, new_word)) {
        return 2;
    } else {
        dict->num_words++;
        dict->words[dict->num_words] = new_word;
        return 1;
    }
}

int ContainsWordArray(DictArray *dict, char* word) {
    if (dict->num_words == 0) {
        return 0;
    }

    int first = 0;
    int last = dict->num_words;
    int middle;

    while (first <= last) {
        middle = (first + last)/2;
        if (strcmp(dict->words[middle], word) == 0) { // Match found
            return 1;
        }
        if (strcmp(dict->words[middle], word) == 1) { // word < current middle
            last = middle - 1;
        }
        if (strcmp(dict->words[middle], word) == -1) {
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
    //char rLetter[2] = {'*', '\0'};
    dictTrie->root = CreateTrieNode(rLetter);
    return dictTrie;
}

void DestroyDictTrie(DictTrie* trie) {

    DestroyTrieNode(trie->root);
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
