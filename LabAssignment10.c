#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[26]; // 26 letters in the English alphabet
    int count; // to keep track of the occurrences of the word
};

// Trie structure
struct Trie
{
    struct TrieNode* root;
};

// Function to create a new Trie node
struct TrieNode* createTrieNode()
{
    struct TrieNode* node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (node)
    {
        for (int i = 0; i < 26; ++i)
            node->children[i] = NULL;
        node->count = 0;
    }
    return node;
}

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* trie = (struct Trie*)malloc(sizeof(struct Trie));
    if (trie)
    {
        trie->root = createTrieNode();
    }
    return trie;
}

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word)
{
    struct TrieNode* node = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
        {
            node->children[index] = createTrieNode();
        }
        node = node->children[index];
    }
    // Mark the end of the word and increase the count
    node->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    struct TrieNode* node = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
        {
            return 0; // The word is not in the Trie
        }
        node = node->children[index];
    }
    return node->count;
}

// Recursive function to deallocate the Trie structure
void deallocateTrieNode(struct TrieNode* node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < 26; ++i)
    {
        deallocateTrieNode(node->children[i]);
    }

    free(node);
}

// Deallocate the trie structure
struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie != NULL)
    {
        deallocateTrieNode(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// This function will return the number of words in the dictionary
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening the file %s\n", filename);
        return 0;
    }

    int numWords = 0;
    char word[256];

    while (fgets(word, sizeof(word), file) != NULL)
    {
        int len = strlen(word);
        if (word[len - 1] == '\n')
            word[len - 1] = '\0'; // Remove the newline character
        pInWords[numWords] = strdup(word); // Store the word in the array
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word into the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL)
        printf("Trie deallocated successfully\n");

    // Deallocate the words array
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}
