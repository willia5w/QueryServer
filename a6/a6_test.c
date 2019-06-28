
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

#define assert(EXPRESSION) (                            \
    (EXPRESSION) ?                                      \
    _assert_pass(#EXPRESSION, __FILE__, __LINE__) :     \
    _assert_fail(#EXPRESSION, __FILE__, __LINE__))

void _assert_fail(const char* expression,
                  const char* file,
                  int line) {
    fprintf(stderr,
            "\n**** Assertion '%s' FAILED, file '%s' line '%d' *******.\n",
            expression,
            file,
            line);
}

void _assert_pass(const char *expression,
                  const char *file,
                  int line) {
    fprintf(stdout,
            "Assertion '%s' passed, file '%s' line '%d'.\n",
            expression,
            file,
            line);
}

void StartTest(const char *test) {
    static int count = 1;
    printf("Test #%d: %s\n\n", count++, test);
}

void EndTest() {
    printf("\n--------------------------------\n\n");
}
/*
void TestCreateTrie() {
  StartTest("CreateTrie");

  DictTrie* trie = CreateDictTrie();
  assert(trie != NULL);
  assert(trie->root != NULL);
  assert(trie->root->my_letter == '*');
  assert(trie->root->is_word == 0);
  DestroyDictTrie(trie);
  EndTest();
}

void TestTrieBasic() {
  StartTest("TrieBasic");

  DictTrie* trie = CreateDictTrie();
  assert(trie != NULL);

  AddWordTrie(trie, "app");
  AddWordTrie(trie, "application");
  AddWordTrie(trie, "butterfly");
  AddWordTrie(trie, "butter");

  assert(ContainsWordTrie(trie, "cat") == 0);
  assert(ContainsWordTrie(trie, "app") == 1);

  DestroyDictTrie(trie);

  EndTest();
}

void TestTrieFile() {
  StartTest("TrieFile");

  DictTrie* trie = LoadDictionaryTrie("wordlist_small.txt");
  assert(trie != NULL);
  assert(ContainsWordTrie(trie, "aals") == 1);
  assert(ContainsWordTrie(trie, "peter") == 0);
<<<<<<< HEAD

  DestroyDictTrie(trie);
    
  EndTest();
}
=======
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035

  EndTest();
}
*/
void TestCreateDictArray() {
    StartTest("CreateDictArray");

    DictArray* array = CreateDictArray();

    assert(array != NULL);

    DestroyDictArray(array);

    EndTest();
}


void TestArrayBasic() {
    StartTest("ArrayBasic");

    DictArray* array = CreateDictArray();

    assert(array != NULL);
    AddWordArray(array, "apple");
    AddWordArray(array, "apples");
    AddWordArray(array, "banana");
    AddWordArray(array, "bespoke");
    AddWordArray(array, "cherry");

    assert(array->num_words == 5);

    assert(ContainsWordArray(array, "apple") == 1);
    assert(ContainsWordArray(array, "cherry") == 1);
    assert(ContainsWordArray(array, "zebra") == 0);
<<<<<<< HEAD
=======

    DestroyDictArray(array);

    EndTest();
}
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035

    DestroyDictArray(array);

<<<<<<< HEAD
    EndTest();
}
=======
/*
void TestArrayFile() {
  StartTest("ArrayFile");

  DictArray* array = LoadDictionaryArray("wordlist_small.txt");
  assert(array != NULL);
  assert(ContainsWordArray(array, "aals") == 1);
  assert(ContainsWordArray(array, "peter") == 0);

  EndTest();
}
*/
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035

/*
void TestArrayFile() {
  StartTest("ArrayFile");

  DictArray* array = LoadDictionaryArray("wordlist_small.txt");
  assert(array != NULL);
  assert(ContainsWordArray(array, "aals") == 1);
  assert(ContainsWordArray(array, "peter") == 0);

  DestroyDictArray(array);
    
  EndTest();
}
<<<<<<< HEAD
=======
*/
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035

int main(void) {
  srand(21774);
    /*
      TestCreateTrie();
      TestTrieBasic();
      TestTrieFile();
      */
      TestCreateDictArray();
      // TestArrayFile();
      TestArrayBasic();

<<<<<<< HEAD
      TestCreateTrie();
      TestTrieBasic();
      TestTrieFile();

      TestCreateDictArray();
      TestArrayFile();
      TestArrayBasic();

=======
>>>>>>> c5f5e214615f940c8c0bef5672ff00caebe1a035
  return EXIT_SUCCESS;
}

