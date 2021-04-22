#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct Bucket{
    struct Bucket *next;
    char * value;
    char * key;
} Bucket;

typedef struct HashMap{
    Bucket** listOfBucket;
    size_t size;
} HashMap;

HashMap* create_hashmap(int key_space);

unsigned int hash(const char *key);

void insert_data(HashMap *hm, const char *key, void *data, void *(*resolve_collision)(void*, void*));

void *get_data(HashMap *hm, const char *key);

void iterate(HashMap *hm, void(*callback)(const char *key, void *data));

void remove_data(HashMap *hm, const char *key, void(DestroyDataCallback)(void *data));

void delete_hashmap(HashMap *hm, void (DestroyDataCallback)(void *data));