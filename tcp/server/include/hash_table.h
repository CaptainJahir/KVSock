#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

typedef struct Hash_Table {
    Node **buckets;
    int size;
} Hash_Table;


Hash_Table *create_hash_table(int size);
bool insert(Hash_Table *table, const char *key, const char *value);
char *search(Hash_Table *table, const char *key);
bool update(Hash_Table *table, const char *key, const char *value);
bool delete(Hash_Table *table, const char *key);

#endif