#include "hash_table.h"

Hash_Table *create_hash_table(int size) {
    if (size <= 0) return NULL;
    
    Hash_Table *table = malloc(sizeof(Hash_Table));
    if (table == NULL) return NULL;
    
    table->size = size;
    table->buckets = calloc(size, sizeof(*table->buckets));
    if (table->buckets == NULL) {
        free(table);
        return NULL;
    }
    
    return table;
}

/* used djb2 hash_function */
unsigned long hash_function(const char *key) {
    unsigned long hash = 5381;
    unsigned char c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}


Node *create_node(const char *key, const char *value) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) return NULL;
    node->key = strdup(key);
    if (node->key == NULL) {
        free(node->key);
        free(node);
        return NULL;
    }
    
    node->value = strdup(value);
    if (node->value == NULL) {
        free(node);
        return NULL;
    }
    node->next = NULL;
    return node;
}

char *search(Hash_Table *table, const char *key) {
    if (table == NULL || key == NULL)
        return NULL;
    unsigned long idx = hash_function(key) % table->size;
    Node *current = table->buckets[idx];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) 
            return current->value;
        current = current->next;
    }
    return NULL;
}

/* TODO: change this from bool to enum so that we can know the exact reason why the insert failed and so that we can forward it to the client */

bool insert(Hash_Table *table, const char *key, const char *value) {
    if (table == NULL || key == NULL || value == NULL)
        return false;
    if (search(table, key))
        return false;
    unsigned long idx = hash_function(key) % table->size;
    Node *entry = create_node(key, value);
    if (entry == NULL) return false;
    entry->next = table->buckets[idx];
    table->buckets[idx] = entry;
    return true;
}

bool update(Hash_Table *table, const char *key, const char *value) {
    if (table == NULL || key == NULL || value == NULL)
        return false;
    unsigned long idx = hash_function(key) % table->size;
    Node *current = table->buckets[idx];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = strdup(value);
            if (current->value == NULL)
                return false;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool delete_node(Hash_Table *table, const char *key) {
    if (table == NULL || key == NULL)
        return false;
    unsigned long idx = hash_function(key) % table->size;
    Node *current = table->buckets[idx];
    Node *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev == NULL) {
                table->buckets[idx] = current->next;
                free(current->key);
                free(current->value);
                free(current);
                return true;
            }
            prev->next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

void clear_table(Hash_Table *table) {
    if (table == NULL) return;

    for (int i = 0; i < table->size; i++) {
        Node *current = table->buckets[i];
        while (current != NULL) {
            Node *next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            current = next;
        }
        table->buckets[i] = NULL;
    }
    return;
}

size_t table_count(const Hash_Table *table) {
    if (table == NULL) return 0;
    size_t count = 0;

    for (int i = 0; i < table->size; i++) {
        Node *current = table->buckets[i];
        while (current != NULL) {
            count++;
            current = current->next;
        }
    }
    
    return count;
}

void list_all(int socket, const Hash_Table *table) {
    if (table == NULL) return;
    char* message;

    size_t item_count = table_count(table);
    if (item_count == 0) {
        message = "No items found";
        send_header_response(socket, item_count, message, SUCCESS);
        return;
    }
        
    message = "Successfully sending items";
    send_header_response(socket, item_count, message, SUCCESS);

    for (int i = 0; i < table->size; i++) {
        Node* current = table->buckets[i];
        while (current != NULL) {
            size_t key_len = strlen(current->key);
            size_t val_len = strlen(current->value);
            size_t arr_len = key_len + val_len;
            Response_Packet* packet = malloc(offsetof(Response_Packet, data) + arr_len);
            if (!packet) {
                fprintf(stderr, "Failed to allocate memory for response packet\n");
                return;
            }
            
            packet->key_len = key_len;
            packet->val_len = val_len;

            memcpy(packet->data, current->key, key_len);
            memcpy(packet->data + key_len, current->value, val_len);

            int sent = send_res_packet(socket, packet, arr_len);
            if (sent < 0) {
                fprintf(stderr, "Failed to send\n");
                current = current->next;
                free(packet);
                continue;
            }
            
            free(packet);
            current = current->next;
        }      
    }
}
