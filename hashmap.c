#include "hashmap.h"


HashMap* create_hashmap(int key_space){
    HashMap *hashMap = malloc(sizeof(HashMap));
    hashMap->listOfBucket = calloc(key_space, sizeof(Bucket));
    hashMap->size = key_space;
    return hashMap;
}

unsigned int hash(const char *key){
    unsigned int h = 0;
    for(size_t i = 0; key[i] != '\0'; i++){
        h += (unsigned int)key[i];
    }
    return h;
}

void insert_data(HashMap *hm, const char *key, void *data, void *(*resolve_collision)(void*, void*)){
    //First hash key
    unsigned int index = hash(key);

    index = index%hm->size;

    Bucket **bucketList = hm->listOfBucket;
    if(bucketList[index] == NULL){
        Bucket *newbucket = malloc(sizeof(Bucket));
        newbucket->value = data;
        newbucket->key = strdup(key);
        newbucket->next = NULL;
        bucketList[index] = newbucket;
    }else {
        Bucket *temp = bucketList[index];
        while (temp != NULL) {
            if (strcmp(temp->key, key) != 0) {
                if(temp->next == NULL) {
                    Bucket *newBucket = malloc(sizeof(Bucket));
                    newBucket->value = data;
                    newBucket->key = strdup(key);
                    newBucket->next = NULL;
                    temp->next = newBucket;
                    break;
                }
            } else {
                temp->value = resolve_collision(temp->value, data);
                break;
            }
            temp = temp->next;
        }
    }
}


void *get_data(HashMap *hm, const char *key){
    unsigned int index = hash(key);

    index = index % hm->size;

    Bucket *bucket = hm->listOfBucket[index];

    while(bucket != NULL){
        if(strcmp(key, bucket->key) == 0){
            return bucket->value;
        }

        if(bucket->next == NULL){
            break;
        }

        bucket = bucket->next;
    }

    return NULL;
}

void iterate(HashMap *hm, void(*callback)(const char *key, void *data)){
    for(size_t i = 0; i < hm->size; i++){
        Bucket *temp = hm->listOfBucket[i];
        while(temp != NULL){
            callback(temp->key, temp->value);
            temp = temp->next;
        }
    }
}

void remove_data(HashMap *hm, const char *key, void (*DestroyDataCallback)(void *data)){

    unsigned int index = hash(key);

    index = index % hm->size;

    Bucket *bucket = hm->listOfBucket[index];
    Bucket *prev = NULL;

    while(bucket != NULL){
        if(strcmp(key, bucket->key) == 0){
            if(prev == NULL){
                hm->listOfBucket[index] = bucket->next;
            }else{
                prev->next = bucket->next;
            }
            if(DestroyDataCallback != NULL){
                DestroyDataCallback(bucket->value);
            }
            free(bucket->key);
            free(bucket);
            return;
        }
        prev = bucket;
        bucket = bucket->next;
    }

}

void delete_hashmap(HashMap *hm, void (*DestroyDataCallback)(void *data)){
    for(size_t i = 0; i < hm->size; i++){
        Bucket *temp = hm->listOfBucket[i];
        Bucket *next;
        while(temp != NULL) {
            next = temp->next;
            if (DestroyDataCallback != NULL) {
                DestroyDataCallback(temp->value);
            }
            free(temp->key);
            free(temp);
            temp = next;
        }
    }
    free(hm->listOfBucket);
    free(hm);
}