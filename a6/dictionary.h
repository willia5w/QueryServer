#ifndef DICT_H
#define DICT_H

#define kMaxNumWords 450000
#define kMaxWordLen 45
#define kNumLetters 26
#define kAlphaStart 97

typedef struct dictTrie DictTrie;

typedef struct trieNode TrieNode;

typedef struct trie Trie;

typedef struct dictArray DictArray;

struct dictArray {
  char* words[kMaxNumWords];
  // Number of words currently in the array
  int num_words; 
};

struct dictTrie {
  TrieNode* root;
};

struct trieNode {
  TrieNode* children[kNumLetters];
  char my_letter;
  int is_word; // 0 for no, 1 for yes
};

// ====================================
// TRIE FUNCTIONS
// ====================================

/**
 * Creates a new Dictionary Trie (DictTrie),
 * allocating space on the heap.
 *
 * Returns a pointer to the newly allocated DictTrie.
 */
DictTrie* CreateDictTrie();

/**
 * Destroys a DictTrie, freeing up all necessary space.
 * Frees all the TrieNodes that are currently in this DictTrie.
 */
void DestroyDictTrie(DictTrie*);

/**
 * Adds the specified word to the provided DictTrie.
 *
 * Param: dict: the DictTrie to add the word to.
 * Param: word: the word to add to the DictTrie.
 *
 * Returns: 1 if the add was successful.
 *          2 if the word is already in the DictTrie.
 *          0 if the add was unsuccessful for some other reason.
 */
int AddWordTrie(DictTrie *dict, char* word);

/**
 * Determines if a given DictTrie contains the provided word.
 *
 * Param: dict: a pointer to the DictTrie to search
 * Param: word: the word to look for in the DictTrie
 *
 * Returns: 0 if dict DOES NOT contain word; 1 if it does.
 */
int ContainsWordTrie(DictTrie* dict, char* word);

/**
 * Creates a TrieNode given the specified letter.
 * Sets all members to default values, except my_letter.
 *
 * Param: letter: the letter represented by this TrieNode.
 *
 * Returns: A pointer to the TrieNode that was newly created.
 */
TrieNode* CreateTrieNode(char letter);

/**
 * Destroys the provided TrieNode.
 *
 */
void DestroyTrieNode(TrieNode* node);

// ====================================
// DICTIONARY ARRAY FUNCTIONS
// ====================================

/**
 * Creates and allocates space for a DictArray.
 *
 * Returns a pointer to the allocated DictArray.
 */
DictArray* CreateDictArray();

/**
 * Destroys the provided DictArray.
 * Also destroys/frees all words that have been added to the DictArray.
 */
void DestroyDictArray(DictArray*);

/**
 * Adds a word to the DictArray.
 * Assumes that the provided word is NOT on the heap,
 * so is responsible for ensuring that space is malloc'd
 * for the given word, and that malloc'd word is stored
 * in the DictArray.
 * After calling AddWordArray(), the caller can assume word is
 * can be freed or modified without impacting the word in the
 * DictArray.
 *
 * Returns: 1 if the add was successful
 *          2 if the word is already in the DictArray
 *          0 if the add was unsuccessful for some other reason.
 */
int AddWordArray(DictArray *dict, char* word);

/**
 * Determines if the given word is in the provided DictArray.
 * Uses BinarySearch to find the word.
 *
 * Param: dict: the DictArray to search (assumes it is alphabetized)
 * Param: word: the word to look for in the DictArray
 *
 * Returns: 1 if the word is found in the DictArray
 *          0 if the word is not found in the DictArray.
 */
int ContainsWordArray(DictArray *dict, char* word);

// ====================================
// HELPER FUNCTIONS
// ====================================

/**
 * Creates and returns a DictTrie that is populated with
 * the words found in the provided file.
 * The callers is responsible for eventually destroying the DictTrie.
 *
 * Param: filename: the file to open and read words from, inserting
 *           each into the DictTrie
 *
 * Returns: a pointer to a malloc'd DictTrie with all the words from
 *           filename added to it.
 */
DictTrie* LoadDictionaryTrie(char *filename);

/**
 * Creates and returns a DictArray that is populated with the
 * words found in the provided file.
 * The caller is responsible for eventually destroying the DictArray.
 *
 * Param: filename: the file to open and read words from, inserting
 *            each into the DictArray.
 *
 * Returns: A pointer to the malloc'd DictArray with all the words
 *           from filename added to it.
 */
DictArray* LoadDictionaryArray(char *filename);

/**
 * Prints out a trie. Helper method. 
 */
void PrintReport(TrieNode* trie);

#endif

