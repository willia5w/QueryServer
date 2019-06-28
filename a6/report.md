How much space does your Trie and Array implementation take (using asymptotic analysis, and calculate in bytes 
for this particular set of words)?

1 Char = 1 byte
- Bytes in Trie: 26^(len of longest word)
- Bytes in Array: (len of longest word) * (num words)

How long (number of cycles and clock time) did it take to create each data structure?
- Trie: 366ms
- Array: 230ms

Overall, which is the better data structure for this use case?
- Array

Is there a use case where you believe the other data structure would be better? Describe it.
- A Trie would be better if all words stemmed from each other, such as with wordlist_small.txt
