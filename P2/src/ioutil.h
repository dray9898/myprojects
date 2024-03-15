#pragma once
#include <stdio.h>
#include <string.h>
#include <unistd.h>   // For read() function
#include <fcntl.h>    // For open() function
#include <stdlib.h>


typedef struct {
  int fd;
  int current_line, current_col;
  int prevword_line, prevword_col;
} File;

char* read_word(File* f);

File* open_file(const char* filename);
void close_file(File* f);