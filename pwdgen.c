#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "c-src/dict.c"
#include "c-src/random.c"

int main(int argc, char* args[])
{
	dict d;
	dicts list;
	char *word;
	int n;

	dict_import("dictionaries/german.dic", &d);

	dict_print(d);

	// TEST MODE, GET NTH WORD FROM DICT

	// while (scanf("%d", &n) > 0 && n >= 0) {
	// 	word = dict_getNthWord(&d, n);
	// 	printf("%s\n====================================\n", word);
	// 	free(word);
	// }
        //
	// return 0;

	/////////////

	list.length = 1;
	list.totalWordCount = d.lines;
	list.dicts = &d;

	word = getRandomWords(4, '-', &list);

	if (word == NULL) {
		printf("Can't get words!\n");
	} else {
		printf("%s\n", word);
	}

	free(word);

	return 0;
}
