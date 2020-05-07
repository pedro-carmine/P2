#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 1023
#define TABLE_SIZE 1000


typedef struct team {
  char * nome;
} Team;

typedef struct game {
  char * nome;
  Team * t1;
  Team * t2;
  int score1, score2;
} Game;

typedef struct entry {
  Team * t;
  int key;
  struct entry *next;
} entry;

typedef struct ht{
  entry **entries;
} ht;

unsigned int hash(const char * key) {
  unsigned long int value = 0;
  unsigned int i = 0;
  unsigned int key_len = strlen(key);

  for (; i < key_len; i++) {
      value = value * 37 + key[i];
  }
  value = value % TABLE_SIZE;
  return value;
}

entry * ht_pair(unsigned int key, char * buffer) {
  entry * entrie = malloc(sizeof(entry));
  entrie->t = malloc(sizeof(Team));
  entrie->t->nome = malloc(sizeof(char) * (strlen(buffer) + 1));
  strcpy(entrie->t->nome, buffer);
  entrie->key = key;

  entrie->next = NULL;
  return entrie;
}

void addTeam(ht * hashtable) {
  unsigned int key;
  char buffer[MAX_CHAR];
  scanf(" %[^:\n]", buffer);
  key = hash(buffer);
  entry * entrie = hashtable->entries[key];

  if (entrie == NULL) {
    hashtable->entries[key] = ht_pair(key, buffer);
    return;
  }

  entry * prev;

  while (entrie != NULL) {
    if (entrie->key == key) {
      free(entrie->t->nome);
      entrie->t->nome = malloc(sizeof(char) * (strlen(buffer) + 1));
      strcpy(entrie->t->nome, buffer);
      return;
    }
    prev = entrie;
    entrie = prev->next;
  }

  prev->next = ht_pair(key, buffer);
}

int ht_get(ht * hashtable) {
  char name[MAX_CHAR];
  scanf(" %[^:\n]", name);
  unsigned int key = hash(name);

  entry * entrie = hashtable->entries[key];

  if (entrie == NULL)
    return -1;

  while (entrie != NULL) {
    if (entrie->key == key) {
      return entrie->key;
    }
    entrie = entrie->next;
  }
  return -1;
}

ht * hash_init() {
  ht * hashtable = malloc(sizeof(ht));
  hashtable->entries = malloc(sizeof(entry*) * TABLE_SIZE);

  int i = 0;
  for(; i < TABLE_SIZE; i++) {
      hashtable->entries[i] = NULL;
  }
  return hashtable;
}


int main() {
  char choice;
  int cmd_count = -1;
  ht * hashtable = hash_init();

  while ((choice = getchar()) != 'x' && choice != EOF) {
    switch (choice) {
      case 'a':
        cmd_count++;
        break;
      case 'A':
        cmd_count++;
        addTeam(hashtable);
        break;
      case 'T':
        cmd_count++;
        printf("%d\n", ht_get(hashtable));
        break;
    }
  }
    return 0;
}
