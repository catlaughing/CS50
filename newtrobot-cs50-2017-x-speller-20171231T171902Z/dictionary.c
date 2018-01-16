/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "dictionary.h"
#define HASHTABLE_SIZE 65536

/**
 * Returns true if word is in dictionary else false.
 */

//define some important variable
bool loaded = false;
int count_word;

//define node
typedef struct node
{
    char word[LENGTH+1];
    struct node *next;
}
node;

node *hashtable[HASHTABLE_SIZE];

//hash function
//shout-out to https://github.com/hathix/cs50-section/blob/master/code/7/sample-hash-functions/good-hash-function.c
unsigned int hash_word(const char* word)
 {
     unsigned long hash = 5381;

     for (const char* ptr = word; *ptr != '\0'; ptr++)
     {
         hash = ((hash << 5) + hash) + tolower(*ptr);
     }

     return hash % HASHTABLE_SIZE;
 }


bool check(const char *word)
{
    // TODO

    //make a copy from word because word is not edit-able
    int len = strlen(word);
    char *word_copy = malloc((sizeof(char) * len) + 1);
    strcpy(word_copy, word);

    //make sure all the letter is lowercase
    for (int i = 0; i < len; i++)
    {
        word_copy[i] = tolower(word_copy[i]);
    }

    //def cursor or temporary pointer
    node *cursor = hashtable[hash_word(word_copy)];

    //looping and compare
    while (cursor != NULL)
    {
        if (strcmp(cursor->word, word_copy) == 0)
        {
            //dont forget to free the memory
            free(word_copy);
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    //still dont forget to free memory
    free(word_copy);
    return false;

}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
     // make all hash table elements NULL
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }

    // open dictionary
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }

    //looping and load
    while (true)
    {
        node *new_node = malloc(sizeof(node));
        //make sure malloc not return a null
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        //read the file into new_node
        fscanf(file, "%s", new_node->word);
        new_node->next = NULL;

        //check if is it the end of the file
        if (feof(file))
        {
            free(new_node);
            break;
        }

        //def a temporary variable to save the hash number
        int temp = hash_word(new_node->word);
        // def a node called head to point to that hashtable
        node* head = hashtable[temp];

        count_word++;

        //check if that table is empty then make that table a pointer to it
        if (head == NULL)
        {
            hashtable[temp] = new_node;
        }
        //if not then make it the first element to it don't care about sorting it takes more time
        else
        {
            new_node->next = hashtable[temp];
            hashtable[temp] = new_node;
        }
    }
    //dont forget to close the file
    fclose(file);
    loaded = true;
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    //just return the count_word
    return count_word;

}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    //looping through the hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        node *cursor = hashtable[i];
        //looping in the hashtable[i]
        while (cursor != NULL )
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    loaded = false;
    return true;
}
