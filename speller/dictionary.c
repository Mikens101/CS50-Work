// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
int counter;
int value;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dict file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Unable to open file %s\n", dictionary);
        return false;
    }

    char word[LENGTH + 1];

    // Read words one at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocate Mem.
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // Create new node per word
        strcpy(n->word, word);

        // Hash word
        n->next = table[hash(word)];
        table[hash(word)] = n;
        counter++;
    }

    fclose(file);
    return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Improve this hash function
    long total = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        long subtotal = tolower(word[i]);
        total += subtotal;
    }
    return total % N;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // return word count
    if (counter > 0)
    {
        return counter;
    }
    return 0;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // chek value
    value = hash(word);
    node *pos = table[value];

    // search list
    while (pos != 0)
    {
        int check = strcasecmp(word, pos->word);
        if (check == 0)
        {
            return true;
        }

        // Next position
        pos = pos->next;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < N; i++)
    {
        node *pos = table[i];

        while (pos)
        {
            node *tmp = pos;
            pos = pos->next;
            free(tmp);
        }

    }
    return true;
}
