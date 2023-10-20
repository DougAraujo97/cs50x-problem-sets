// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

int wordcount = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = (LENGTH + 2) * 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hashes the input word and checks the dictionary for matches
    int hash_index = hash(word);
    node *cursor = table[hash_index];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Hashes words based on length and first letter
    int length = strlen(word) + 1;
    char x = tolower(word[0]);
    int alphindex = x - 'a';
    int hash_index = alphindex * (LENGTH + 1) + length;
    return hash_index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char scan_word[LENGTH + 1];

    // open the file
    FILE *input = fopen(dictionary, "r");

    // check if NULL
    if (input == NULL)
    {
        return false;
    }

    // read through each string and create a node for it
    while (fscanf(input, "%s", scan_word) != EOF)
    {
        node *new_word = malloc(sizeof(node));

        if (new_word == NULL)
        {
            return false;
        }

        strcpy(new_word->word, scan_word);

        // hash the string
        int hash_index = hash(scan_word);

        // place the string into the hash table
        if (table[hash_index] == NULL)
        {
            new_word->next = NULL;
        }

        else
        {
            new_word->next = table[hash_index];
        }

        table[hash_index] = new_word;

        // Update the wordcount
        wordcount++;

    }

    // close the file and return
    fclose(input);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // returns the wordcount the program has kept track of
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterates through table array
    for (int i = 0; i <= N; i++)
    {
        node *cursor = table[i];
        node *temp = table[i];

        // Frees every node in every linked list
        while (temp != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }

    }

    return true;
}
