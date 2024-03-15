#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"
#include "ioutil.h"

char* capitalize(const char* word) {
    size_t sz = strlen(word)+1;
    char* newword = calloc(sz, sizeof(char)); 
    memcpy(newword, word, sz);
    if (isalpha(newword[0]) && !isupper(newword[0])) newword[0] -= 32;
    return newword;
}

char* uppercase(const char* word) {
    char* newword = calloc(strlen(word)+1, sizeof(char)); int c = 0;
    while (*word != 0) {
        if (isupper(*word)) {
            newword[c++] = *word;
        } else if (isalpha(*word)) {
            newword[c++] = *word - 32;
        }
        word++;
    }
    return newword;
}

int compare_strings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

Dictionary* load_dictionary(const char *dictionary_path) {
    File* dictionaryFile;
    dictionaryFile = (File *) open_file(dictionary_path);
    
    int wordCount=0;
    int dataCapacity= 8;
    Dictionary* newDict = malloc(sizeof(Dictionary)); 
       if (newDict == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        close_file(dictionaryFile);
        return NULL;
    }
    
    newDict->listOfwords = malloc(dataCapacity*sizeof(char*));
       if (newDict == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        close_file(dictionaryFile);
        return NULL;
    }

    char* word;
    while ((word = read_word(dictionaryFile)) != NULL) {
        if (wordCount >= dataCapacity - 2) {
            dataCapacity *=2;
            newDict->listOfwords = realloc(newDict->listOfwords, dataCapacity*sizeof(void*));
            
            if (newDict->listOfwords == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                close_file(dictionaryFile);
                free(newDict);
                return NULL;
            }
        }
        newDict->listOfwords[wordCount++] = word; // Post decrement 
        newDict->listOfwords[wordCount++] = uppercase(word);
        newDict->listOfwords[wordCount++] = capitalize(word);
    }

    newDict->length = wordCount;
   
    qsort(newDict->listOfwords, wordCount, sizeof(char*), compare_strings);
    
    close_file(dictionaryFile);
    return newDict;
}

bool is_word(Dictionary* dict, const char* pointer) {
    int left = 0;
    int right = dict->length - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(dict->listOfwords[mid], pointer);
        
        if (cmp == 0) {
            return true; 
        } else if (cmp < 0) {
            left = mid + 1; 
        } else {
            right = mid - 1; 
        }
    }
    return false;
}

void free_dictionary(Dictionary* dict) {
    for (int i = 0; i < dict->length; i++) {
        free(dict->listOfwords[i]);
    }
    free(dict->listOfwords);
    free(dict);
}

/*
int main(int argc, char **argv){
 
    Dictionary* dict = load_dictionary("/common/users/shared/cs214s24pa1dr101ezy1/P2/src/dictionary.txt");
     int s = dict->length;
     
      for (int i = 0; i < s; i++) {
            printf("%s\n", dict->listOfwords[i]);
        }
        for (int i = 0; i < s; i++) {
            bool a = is_word(dict,dict->listOfwords[i]);
            printf("%d\n", a);
        }
    
        bool a = is_word(dict,"Acerola");
        printf("%d\n", a);


    }
    */