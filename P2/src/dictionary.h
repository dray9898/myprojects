#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "ioutil.h"


typedef struct {
  char** listOfwords;
  int length;
} Dictionary;

Dictionary* load_dictionary(const char*);
bool is_word(Dictionary*, const char*);
void free_dictionary(Dictionary*);