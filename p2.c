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

typedef struct node {
  Team * t;
  struct node *next;
} node;

typedef struct {
  node **entries;
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

node * push(node * head, char * s) {
  node * new = malloc(sizeof(node));
  new->t = malloc(sizeof(Team));
  new->t->nome = malloc(sizeof(char) * (strlen(s) + 1));
  strcpy(new->t->nome, s);
  new->next = head;
  return new;
}

void print_team(node * head, int cmd_count) {
  node * aux;
  aux = head;
  printf("%d %s\n", cmd_count, aux->t->nome);
}

void addTeam(ht * table) {
  int key;
  node* aux;
  char team1_buffer[MAX_CHAR];
  scanf(" %[^:\n]", team1_buffer);
  key = hash(team1_buffer);
  while (table->entries[key]->next!=NULL) {
    aux=table->entries[key]->next;
}
  aux = aux->next;
}

ht* hash_init() {
  int i = 0;
  ht *table= malloc(sizeof(ht));
  for (; i < TABLE_SIZE; i++) {
    table->entries[i] = malloc(sizeof(struct node));
  }
return table;
}


int main() {
  char choice;
  int cmd_count = -1;
  ht * table = hash_init();
  table->entries = malloc(sizeof(node));

  while ((choice = getchar()) != 'x' && choice != EOF) {
    switch (choice) {
      case 'a':
        cmd_count++;
        break;
      case 'A':
        cmd_count++;
        addTeam(table);
        break;
      case 'T':
        break;
    }
  }
    return 0;
}
