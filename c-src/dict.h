#ifndef DICT_H_INCL
#define DICT_H_INCL

#include <sodium.h>

#define DICT_NAME_MAX_LENGTH 64

// get the right path separator
#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif //_WIN32

typedef struct dict_ {
  unsigned int lines;
  char name[DICT_NAME_MAX_LENGTH + 1];
  char hash[65];
} dict;

typedef struct dict_list {
  int length;
  long int totalWordCount;
  char * path;
  dict * dicts;
} dicts;

int dict_create(dict *d, int lines, char * name, char hash[65]);

int dict_set_lines(dict *d, unsigned int lines);

int dict_set_name(dict *d, char * name);

int dict_set_hash(dict *d, char hash[65]);

int dict_update(dict d);

char * dict_getNthWord(dict *d, int n);

int dict_getHashAndLineCount (char* path, char hash[65], unsigned int * linecount);

int dict_import(char * path, dict *d);

int dicts_check(dicts list);

int dicts_write(dicts list);

int dicts_add(dicts list, dict *d);

int dicts_remove(dicts list, dict *d);

char * filenameFromPath(char * path);

void dict_print(dict d);

#endif
