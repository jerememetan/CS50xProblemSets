#include "dictionary.h"
#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int sizeofd = 0;
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1]; // array of chars + \n, eg CAT is char[4]
    struct node *next;
} node;

void free_nodes(node *n);
const unsigned int N = 26 * 26;

// Hash table
node *table[N]; // this will be used to keep track of words in the dictionary

// Returns true if word is in dictionary, else false
bool check(const char *word) // FOURTH
{

    int hashvalue = hash(word);
    node *pointer;

    pointer = table[hashvalue];
    if (pointer == NULL)
    {

        return false;
    }
    while (pointer->next != NULL)
    {
        if (strcasecmp(word, pointer->word) == 0)
        {

            return true;
        }
        else
        {
            pointer = pointer->next;
        }
    }
    while (pointer->next == NULL)
    {
        if (strcasecmp(word, pointer->word) == 0)
        {
            return true;
        }
        return false;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word) // SECOND
{
    int values[2];
    int hashvalue;
    for (int i = 0; i < 2; i++)
    {
        if (isalpha(word[i]) && isupper(word[i]))
        {
            values[i] = word[i] - 'A';
        }
        else if (isalpha(word[i]))
        {
            values[i] = word[i] - 'a';
        }
        else
        {
            values[i] = 0;
        }
    }
    hashvalue = (values[0] * 26) + (values[1]);
    return hashvalue;
}

// Loads dictionary into table, returning true if successful, else false
bool load(const char *dictionary) // FIRST
{
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        return false;
    }
    char tempword[LENGTH];
    // read file 1 string at a time
    while (fscanf(source, "%s", tempword) != EOF)
    {
        sizeofd++;
        node *newnode = malloc(LENGTH * sizeof(node)); // create a temp node
        if (newnode == NULL)
        {
            return false;
        }
        strcpy(newnode->word, tempword); // define node accordingly
        newnode->next = NULL;
        int tablepos = hash(tempword);
        if (table[tablepos] == NULL)
        {
            table[tablepos] = newnode;
        }
        else
        {
            newnode->next = table[tablepos];
            table[tablepos] = newnode;
        }
    }

    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void) // THIRD
{

    return sizeofd;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void) // FIFTH
{
    // free all the loaded nodes inside table
    for (int i = 0; i < N; i++)
    {
        free_nodes(table[i]);
    }
    return true;
}

void free_nodes(node *n)
{
    if (n == NULL)
    {
        return;
    }
    free_nodes(n->next);
    free(n);
}
