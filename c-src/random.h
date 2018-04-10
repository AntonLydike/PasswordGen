#ifndef MY_RANDOM_H_INCL
#define MY_RANDOM_H_INCL

#include <stdint.h>
#include "dict.h"

char * getRandomWord(dicts *list);

char * getRandomWords(int wordcount, char delimiter, dicts *list);

#endif
