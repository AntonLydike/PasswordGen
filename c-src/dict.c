#include "dict.h"


// #define READ_BUFFER_SIZE 4096
#define READ_BUFFER_SIZE 24


int dict_create(dict *d, int lines, char * name, char hash[65])
{
        if (!dict_set_lines(d, lines)
         || !dict_set_name(d, name)
         || !dict_set_hash(d, hash)) {
                 return 0;
        }

        return 1;
}

int dict_set_lines(dict *d, unsigned int lines)
{
        d->lines = lines;
        return 1;
}

int dict_set_name(dict *d, char * name)
{
        if (strlen(name) > DICT_NAME_MAX_LENGTH) {
                printf("[Warn] Dictionary name longer than allowd length of %d: %s", DICT_NAME_MAX_LENGTH, name);
        }
        strncpy(d->name, name, DICT_NAME_MAX_LENGTH);
        d->name[DICT_NAME_MAX_LENGTH] = '\0';

        return 1;
}

int dict_set_hash(dict *d, char hash[65])
{
        strncpy(d->hash, hash, 64);
        d->hash[64] = '\0';
        return 1;
}

int dict_update(dict d)
{

}

char * dict_getNthWord(dict *d, int n)
{
        unsigned int i, bytesRead, lines, carrylineover;
        char *buffer, *searchbuff, *result, *tmpresult, *tmprealloc;
        FILE* file;

        if (n >= d->lines) {
                printf("Requested line %d while dictionary only has %d lines\n", n, d->lines);
                return NULL;
        }

        // "dictionaries/" + DICT_NAME_MAX_LENGTH + '\0'
        char path[13 + DICT_NAME_MAX_LENGTH + 1];
        path[0] = '\0';
        strcat(path, "dictionaries/");
        strcat(path, d->name);

        file = fopen(path, "r");
        if(!file) {
            printf("File \"%s\" not found!\n", path);
            return NULL;
        }

        buffer = malloc(READ_BUFFER_SIZE);
        if(!buffer) {
            printf("Couldn't allocate enought memory to open \"%s\"!\n", path);
            fclose(file);
            return NULL;
        }

        bytesRead     = 0;
        lines         = 0;
        carrylineover = 0;

        while ((bytesRead = fread(buffer, 1, READ_BUFFER_SIZE, file))) {
                searchbuff = buffer;

                // carrylineover holds the current length of the result
                // (the buffer is one longer, because of the terminating null)
                if (carrylineover != 0) {
                        // in this case buffer points to the beginning, so we don't need tmpresult to also
                        // point there

                        // don't look if already found
                        if (*searchbuff != '\n') {
                                //look for next terminating newline (assuming no word is longer than 4097 characetrs)
                                while (*(++searchbuff) != '\n');
                        }

                        // allocate currently needed space + space til next newline (replace newline with null)
                        tmprealloc = realloc(result, carrylineover + searchbuff - buffer);
                        if (tmprealloc == NULL) {
                                printf("Not enough memory to save word! (realloc)\n");
                                fclose(file);
                                free(buffer);
                                free(result);
                                return NULL;
                        }
                        result = tmprealloc;
                        strncat(result, buffer, searchbuff - buffer);

                        fclose(file);
                        free(buffer);
                        return result;

                } else if (n == lines) {
                        // look for newline
                        while (*(++searchbuff) != '\n');
                        result = malloc(searchbuff - buffer + 1);
                        strncpy(result, buffer, searchbuff - buffer);
                        result[searchbuff - buffer] = '\0';

                        fclose(file);
                        free(buffer);
                        return result;
                }

                while ((searchbuff = memchr(searchbuff, '\n', bytesRead - (searchbuff - buffer)))) {
                        searchbuff++;
                        lines++;

                        if (lines == n) {
                                // tmpresult points to begin of desired line
                                tmpresult = searchbuff;
                                // look for newline or end of chunk read
                                while(searchbuff - buffer < READ_BUFFER_SIZE && *(++searchbuff) != '\n');
                                // allocate ram for required length
                                // (+1 to copy including the last character)
                                // (+1 for terminating null)
                                result = malloc(searchbuff - tmpresult + 2);
                                if (result == NULL) {
                                        printf("Not enough memory to save word!\n");
                                        fclose(file);
                                        free(buffer);
                                        return NULL;
                                }
                                // copy found bytes over to result buffer
                                // including the one pointed to by searchbuff
                                strncpy(result, tmpresult, searchbuff - tmpresult);
                                // place null terminator behind the last copied character
                                result[searchbuff - tmpresult] = '\0';

                                // if we hit the buffer size boundary
                                if (*searchbuff != '\n') {
                                        // continue and save bytes copied
                                        carrylineover = searchbuff - tmpresult - 1;
                                } else {
                                        // otherwise free resources and return pointer
                                        fclose(file);
                                        free(buffer);
                                        return result;
                                }
                        }
                }
        }

        // we should never get here...
        fclose(file);
        free(buffer);
        printf("Couldn't find line %d in file \"%s\"\n", n, path);
        return NULL;
}

int dict_import(char * path, dict *d)
{
        unsigned int lines;
        char hash[65];
        char * name = filenameFromPath(path);

        // get hash and line count
        if (!dict_getHashAndLineCount(path, hash, &lines)) {
                return 0;
        }

        return dict_create(d, lines, name, hash);
}

int dicts_check(dicts list)
{

}

int dicts_write(dicts list)
{

}

int dicts_add(dicts list, dict *d)
{

}

int dicts_remove(dicts list, dict *d)
{

}

int dict_getHashAndLineCount (char* path, char hash[65], unsigned int * linecount)
{
        unsigned int i, bytesRead, lines;
        unsigned char out[crypto_hash_sha256_BYTES], *buffer, *searchbuff, lastchar;
        FILE* file;
        crypto_hash_sha256_state state;

        if (sodium_init() < 0) {
            printf("Libsodium couldn't be initialized to hash \"%s\"!\n", path);
            return 0;
        }

        if (crypto_hash_sha256_init(&state)) {
            printf("Couldn't initialize SHA256 for \"%s\"!\n", path);
            return 0;
        }

        file = fopen(path, "rb");
        if(!file) {
            printf("File \"%s\"not found!\n", path);
            crypto_hash_sha256_final(&state, out);
            return 0;
        }

        buffer = malloc(READ_BUFFER_SIZE);
        if(!buffer) {
            printf("Couldn't allocate enought memory to hash \"%s\"!\n", path);
            fclose(file);
            crypto_hash_sha256_final(&state, out);
            return 0;
        }

        bytesRead = 0;
        lines     = 0;

        while ((bytesRead = fread(buffer, 1, READ_BUFFER_SIZE, file))) {
                lastchar = buffer[bytesRead - 1];
                searchbuff = buffer;

                while ((searchbuff = memchr(searchbuff, '\n', bytesRead - (searchbuff - buffer)))) {
                        searchbuff++;
                        lines++;
                }

                if (crypto_hash_sha256_update(&state, buffer, bytesRead)) {
                        printf("Error while hashing \"%s\"\n", path);
                        crypto_hash_sha256_final(&state, out);
                        fclose(file);
                        free(buffer);
                        return 0;
                }
        }

        if (lastchar != '\n') {
                lines++;  /* Count the last line even if it lacks a newline */
        }

        if (crypto_hash_sha256_final(&state, out)) {
                printf("final\n");
                fclose(file);
                free(buffer);
                return 0;
        }

        for(i = 0; i < crypto_hash_sha256_BYTES; i++) {
                sprintf(hash + (i * 2), "%02x", out[i]);
        }

        hash[64] = 0;
        *linecount = lines;

        fclose(file);
        free(buffer);
        return 1;
}

char * filenameFromPath(char * path)
{
        int pos = strlen(path) - 1;

        // as long as the path ends in a slash, it's not the filename
        while (pos > 0 && path[pos] == PATH_SEPARATOR) pos--;

        for (; pos > 0 && path[pos] != PATH_SEPARATOR; pos--);

        return path + (pos + 1);
}

void dict_print(dict d)
{
        printf("%16s (%7d): %s\n", d.name, d.lines, d.hash);
}

int is_line_separator(char c)
{
        return c == '\n' || c == '\r';
}
