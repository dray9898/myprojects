#include "ioutil.h"

  char* read_word(File* f) {
    char* buf = malloc(16);
    if (buf == NULL) {
        fprintf(stderr, "ERROR in malloc\n");
        return NULL;
    }

    int currentSize = 16;
    int counter = 0;
    char c;
    int fd = f->fd;

    while (read(fd, &c, 1) == 1) {
        if (counter == 0) {
            f->prevword_col = f->current_col;
            f->prevword_line = f->current_line;
        }

        if (c == ' ' || c == '\t' || c == '\n') {
            if (c == ' ') {
                f->current_col++;
            } else if (c == '\t') {
                f->current_col += 4; // Assuming tab width is 4
            } else if (c == '\n') {
                f->current_col = 1;
                f->current_line++;
            }
            if (counter > 0) break;
        } else {
            buf[counter++] = c;
            if (counter == currentSize - 1) {
                currentSize *= 2;
                buf = realloc(buf, currentSize);
                if (buf == NULL) {
                    fprintf(stderr, "ERROR in realloc\n");
                    free(buf);
                    return NULL;
                }
            }
            f->current_col++;
        }
    }

    if (counter <= 0) {
        free(buf);
        return NULL;
    }


    buf[counter] = '\0';
    return buf;
}

File* open_file(const char* filename) {
  File* f = malloc(sizeof(File));
  f->current_col = 1;
  f->current_line = 1;
  if (f == NULL) {
    fprintf(stderr, "could not allocate %s", filename);
    exit(1);
    
  }
  int res;
  if ((res = open(filename, O_RDONLY)) >= 0) {
    f->fd = res;
    return f;
  }
  fprintf(stderr, "could not open file %s", filename);
  perror("");
  free(f);
  return NULL;
}

void close_file(File* f) {
  close(f->fd);
  free(f);
}

