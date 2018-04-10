#include "random.h"
#include <sodium.h>
#include <string.h>
#include <stdio.h>


/*
 *      Get a random word from a random dictionary
 *      ! THE STRING HAS TO BE FREE'D WHEN FINISHED !
 *
 */
char * getRandomWord(dicts *list)
{
        int dict = 0;
        uint32_t random;

        if (sodium_init() < 0) {
                printf("Libsodium couldn't be initialized!\n");
                exit(1);
        }

        // random = randombytes_uniform((uint32_t) list->totalWordCount);

        printf("Random number: ");
        unsigned long int r;
        while (scanf("%lu", &r) != 1);
        random = (uint32_t) r;

        while (dict < list->length - 1 && list->dicts[dict].lines < random) {
                random -= list->dicts[dict].lines;
                dict++;
        }

        return dict_getNthWord(&(list->dicts[dict]), random);
}

char * getRandomWords(int wordcount, char delimiter, dicts *list)
{
        char *words, *tmp, *current;
        int wordlen, length = 1;

        // reserve one char to get started
        words = malloc(1 * sizeof(char));
        // and make it an empty string

        if (words == NULL) {
                printf("Error while allocating memory!\n");
                return NULL;
        }

        // place null
        words[0] = '\0';

        while (wordcount > 0) {
                current = getRandomWord(list);
                length += 1 + strlen(current);
                tmp = realloc(words, length + 1);

                printf("got word: %s\n", current);

                if (tmp == NULL) {
                        printf("Cannot allocate enought space: %d.\n", length);
                        free(words);
                        return NULL;
                }
                words = tmp;

                strcat(words, current);

                if (wordcount > 1) {
                        words[length - 2] = delimiter;
                        words[length - 1] = '\0';
                }

                printf("Status: %s\n", words);

                // free the now copied word
                free(current);
                wordcount--;
        }

        return words;
}
