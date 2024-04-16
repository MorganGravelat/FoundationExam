#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct KeyValuePair {
    char key[50];
    int value;
    struct KeyValuePair* next;
};

struct HashTable {
    int size;
    struct KeyValuePair** table;
};

struct KeyValuePair* createKeyValuePair(const char* key, int value) {
    struct KeyValuePair* newPair = (struct KeyValuePair*)malloc(sizeof(struct KeyValuePair));
    if (newPair != NULL) {
        strcpy(newPair->key, key);
        newPair->value = value;
        newPair->next = NULL;
    }
    return newPair;
}

struct HashTable* createHashTable(int size) {
    struct HashTable* newTable = (struct HashTable*)malloc(sizeof(struct HashTable));
    if (newTable != NULL) {
        newTable->size = size;
        newTable->table = (struct KeyValuePair**)calloc(size, sizeof(struct KeyValuePair*));
    }
    return newTable;
}

unsigned int hashFunction (const char* key, int tableSize) {//p u f f \0
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;
    }
    return hash % tableSize;
}

void insert(struct HashTable* hashTable, const char* key, int value) {
    unsigned int index = hashFunction(key, hashTable->size);
    struct KeyValuePair* newPair = createKeyValuePair(key, value);
    newPair->next = hashTable->table[index];
    hashTable->table[index] = newPair;
}

int retrieve(struct HashTable* hashTable, const char* key) {
    unsigned int index = hashFunction(key, hashTable->size);
    struct KeyValuePair* current = hashTable->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return -1; // Return -1 if the key is not found
}

void removeKey(struct HashTable* hashTable, const char* key) {
    unsigned int index = hashFunction(key, hashTable->size);
    struct KeyValuePair* current = hashTable->table[index];
    struct KeyValuePair* previous = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (previous == NULL) {
                hashTable->table[index] = current->next;
            } else {
                previous->next = current->next;
            }

            free(current);
            return;

        }
        previous = current;
        current = current->next;
    }
}

void displayHashTable(struct HashTable* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        printf("[%d] -> ",i);

        struct KeyValuePair* current = hashTable->table[i];
        while (current != NULL) {
            printf("(%s, %d) -> ", current->key, current->value);
            current = current->next;
        }

        printf("NULL\n");
    }
}

void freeHashTable(struct HashTable* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        struct KeyValuePair* current = hashTable->table[i];
        while (current != NULL) {
            struct KeyValuePair* temp = current;
            current = current->next;
            free(temp);

        }
    }

    free(hashTable->table);
    free(hashTable);
}

int main() {
    struct HashTable* hashTable = createHashTable(10);

    insert(hashTable, "Alaric", 35);
    insert(hashTable, "Faustus", 28);
    insert(hashTable, "Wilbur", 30);
    insert(hashTable, "Melcha", 31);

    printf("Initial Hash Table:\n");
    displayHashTable(hashTable);
    printf("\n");

    printf("Value for key 'Alaric': %d\n", retrieve(hashTable, "Alaric"));
    printf("Value for key 'Faustus': %d\n", retrieve(hashTable, "Faustus"));
    printf("Value for key 'Cambria': %d\n", retrieve(hashTable, "Cambria"));
    printf("\n");

    // Remove a key-value pair and display the updated hash table
    printf("Deleting key 'Faustus'\n");
    removeKey(hashTable, "Faustus");
    displayHashTable(hashTable);
    printf("\n");

    // Free the memory allocated for the hash table
    freeHashTable(hashTable);

    return 0;
}
