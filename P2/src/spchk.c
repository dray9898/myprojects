#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>

#include "spchk.h"
#include "ioutil.h"

#ifndef DEBUG
  #define DEBUG 0
#endif

bool parse(Dictionary* d, const char* fname) {
  bool errors = false;
  struct stat buf;
  stat(fname, &buf);
  if (S_ISDIR(buf.st_mode)) {
    errors |= parse_folder(d, fname);
  } else if (S_ISREG(buf.st_mode)) {
    errors |= parse_file(d, fname);
  } else {
    fprintf(stderr, "skipping non-file \"%s\"\n", fname);
  }
  return errors;
}

bool parse_folder(Dictionary* dict, const char* fname) {
  DIR* d = opendir(fname);
  if (d == NULL) {
    fprintf(stderr, "could not open folder %s", fname);
    perror("");
    return true;
  }
  struct dirent* dr;
  bool errors = false;
  while ((dr = readdir(d)) != NULL) {
    if (strcmp(dr->d_name, ".") == 0 || strcmp(dr->d_name, "..") == 0) continue;
    int len = strlen(fname) + 2 + strlen(dr->d_name);
    char* newf = malloc(len);
    snprintf(newf, len, "%s/%s", fname, dr->d_name);
    errors |= parse(dict, newf);
    free(newf);
  }
  closedir(d);
  return errors;
}

bool parse_file(Dictionary* d, const char* fname) {
  File* f = open_file(fname);
  if (f == NULL) return true;
  char* word;
  bool errors = false;
  while ((word = read_word(f)) != NULL) {
    if (!spellcheck(d, word)) {
      errors = true;
      printf("%s (%d,%d): %s\n", fname, f->prevword_line, f->prevword_col, word);
    }
    free(word);
  }
  close_file(f);
  return errors;
}

bool spellcheck(Dictionary* d, const char* word) {
  if (DEBUG) printf("Checking \"%s\"\n", word);
  bool seen_letter = false;
  bool passed = true;
  int N = strlen(word);
  char* sanitized = calloc(N + 1, sizeof(char));
  char* term = NULL;
  int c = 0;
  while (*word != '\0') {
    if (!seen_letter) {
      if (*word == '{' || *word == '(' || *word == '[' || *word == '\'' || *word == '"') {
        word++;
        continue;
      }
    }
    if (isalpha(*word)) {
      seen_letter = true;
      term = NULL;
      sanitized[c++] = *word;
    } else if (*word == '-') {
      sanitized[c++] = '\0'; // do not remove "trailing" punctuation
      c = 0;
      passed &= is_word(d, sanitized);
    } else {
      if (term == NULL) {
        term = &sanitized[c];
      }
      sanitized[c++] = *word;
    }
    ++word;
  } 
  if (DEBUG) printf("trimming '%s'\n", sanitized);
  if (term != NULL) {
    if (DEBUG) printf("term points to '%c'\n", *term);
    *term = '\0';
  }
  if (passed && *sanitized != '\0') {
    if (DEBUG) printf("checking '%s'\n", sanitized);
    passed = is_word(d, sanitized);
  }
  free(sanitized);
  return passed;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("usage: %s [dictionary file] [list of files...]\n", argv[0]);
    return 0;
  }
  Dictionary* dict = load_dictionary(argv[1]);
  bool errors = false;
  for (int i = 2; i < argc; i++) {
    errors |= parse(dict, argv[i]);
  }
  free_dictionary(dict);
  if (errors) {
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}