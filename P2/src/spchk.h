#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "dictionary.h"

bool parse(Dictionary*,const char*);
bool parse_folder(Dictionary*,const char*);
bool parse_file(Dictionary*, const char*);
bool spellcheck(Dictionary*, const char*);