#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 1023
#define TABLE_SIZE 100


typedef struct team {
  char * nome;
  int wins;
} Team;

typedef struct game {
  char * gameName;
  Team * t1;
  Team * t2;
  int score1, score2;
} Game;

typedef struct entry {
  Team * t;
  Game * game;
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

ht * hash_init() {
  int i;
  ht * hashtable = malloc(sizeof(ht));
  hashtable->entries = malloc(sizeof(entry*) * TABLE_SIZE);

  i = 0;
  for(; i < TABLE_SIZE; i++) {
      hashtable->entries[i] = NULL;
  }
  return hashtable;
}

entry * ht_pair_team(unsigned int key, char * buffer) {
  entry * entrie = malloc(sizeof(entry));
  entrie->t = malloc(sizeof(Team));
  entrie->t->nome = malloc(sizeof(char) * (strlen(buffer) + 1));
  entrie->t->wins = 0;
  strcpy(entrie->t->nome, buffer);
  entrie->key = key;
  entrie->next = NULL;

  return entrie;
}

entry * ht_pair_game(unsigned int key, char * buffer, Team * t1, Team * t2, int score1, int score2) {
  entry * entrie = malloc(sizeof(entry));
  entrie->game = malloc(sizeof(Game));
  entrie->game->gameName = malloc(sizeof(char) * (strlen(buffer) + 1));
  entrie->game->t1 = t1;
  entrie->game->t2 = t2;
  entrie->game->score1 = score1;
  entrie->game->score2 = score2;
  strcpy(entrie->game->gameName, buffer);
  entrie->key = key;
  entrie->next = NULL;

  if (score1 > score2) {
    t1->wins++;
  }
  else if (score2 > score1) {
    t2->wins++;
  }
  return entrie;
}

void addTeam(ht * hashtable, int cmd_count) {
  char teamname[MAX_CHAR];
  entry * entrie;
  entry * prev;
  unsigned int key;
  scanf(" %[^:\n]", teamname);
  key = hash(teamname);
  entrie = hashtable->entries[key];

  if (entrie == NULL) {
    hashtable->entries[key] = ht_pair_team(key, teamname);
    return;
  }


  while (entrie != NULL) {
    if (strcmp(entrie->t->nome, teamname) != 0 && entrie->next == NULL) {
      entrie->next = ht_pair_team(key, teamname);
      return;
    }
    else if (strcmp(entrie->t->nome, teamname) == 0) {
      printf("%d Equipa existente.\n", cmd_count);
      return;
    }

    prev = entrie;
    entrie = prev->next;
  }

  prev->next = ht_pair_team(key, teamname);
}


Team * teamSearch(ht * hashtable, char * name) {
  entry * entrie;
  unsigned int key;
  key = hash(name);

  entrie = hashtable->entries[key];

  if (entrie == NULL) {
    return NULL;
  }
  while (entrie != NULL) {
    if (strcmp(entrie->t->nome, name) == 0) {
      return entrie->t;
    }
    entrie = entrie->next;
  }
  return NULL;
}

Game * gameSearch(ht * game_ht, char * name) {
  entry * entrie;
  unsigned int key;
  key = hash(name);

  entrie = game_ht->entries[key];

  if (entrie == NULL) {
    return NULL;
  }
  while (entrie != NULL) {
    if (strcmp(entrie->game->gameName, name) == 0) {
      return entrie->game;
    }
    entrie = entrie->next;
  }
  return NULL;
}

void getTeam(ht * hashtable, int cmd_count) {
  char name[MAX_CHAR];
  Team * team;
  scanf(" %[^:\n]", name);
  team = teamSearch(hashtable, name);

  if (team == NULL) {
    printf("%d Equipa inexistente.\n", cmd_count);
  }
  else {
    printf("%d %s %d\n", cmd_count, team->nome, team->wins);
  }
}

void getGame(ht * game_ht, int cmd_count) {
  char name[MAX_CHAR];
  Game * game;
  scanf(" %[^:\n]", name);

  game = gameSearch(game_ht, name);

  if (game == NULL) {
    printf("%d Jogo inexistente.\n", cmd_count);
  }
  else {
    printf("%d %s %s %s %d %d\n", cmd_count, game->gameName, game->t1->nome, game->t2->nome, game->score1, game->score2);
  }
}

void addGame(ht * hashtable, ht * game_ht, int cmd_count) {
  char gameName[MAX_CHAR];
  char t1_name[MAX_CHAR];
  char t2_name[MAX_CHAR];
  int score1, score2;
  Team * t1;
  Team * t2;
  entry * entrie;
  entry * prev;
  unsigned int key;

  scanf(" %[^:]:%[^:]:%[^:]:%d:%d", gameName, t1_name, t2_name, &score1, &score2);
  t1 = teamSearch(hashtable, t1_name);
  t2 = teamSearch(hashtable, t2_name);

  key = hash(gameName);
  entrie = game_ht->entries[key];

  if (entrie == NULL) {
    if (t1 == NULL || t2 == NULL) {
      printf("%d Equipa inexistente.\n", cmd_count);
      return;
    }
    game_ht->entries[key] = ht_pair_game(key, gameName, t1, t2, score1, score2);
    return;
  }

  while (entrie != NULL) {
    if (strcmp(entrie->game->gameName, gameName) != 0 && entrie->next == NULL && t1 != NULL && t2 != NULL) {
      entrie->next = ht_pair_game(key, gameName, t1, t2, score1, score2);
      return;
    }
    else if (strcmp(entrie->game->gameName, gameName) == 0) {
      printf("%d Jogo existente.\n", cmd_count);
      return;
    }
    else if (t1 == NULL || t2 == NULL) {
      printf("%d Equipa inexistente.\n", cmd_count);
      return;
    }
    prev = entrie;
    entrie = prev->next;
  }
  prev->next = ht_pair_game(key, gameName, t1, t2, score1, score2);
}

void destroyTeam(ht * hashtable) {
  int i = 0;
  for (i = 0; i < TABLE_SIZE; i++) {
    printf("for\n");
    while (hashtable->entries[i]->next != NULL) {
      printf("while\n");
      free(hashtable->entries[i]->next->t->nome);
      free(hashtable->entries[i]->next->t);
    }
    printf("saiu while\n");
    free(hashtable->entries[i]->t->nome);
    free(hashtable->entries[i]->t);
    free(hashtable->entries[i]);
  }
}

void destroyGame(ht * hashtable) {
  int i = 0;
  for (i = 0; i < TABLE_SIZE; i++) {
    while (hashtable->entries[i]->next != NULL) {
    free(hashtable->entries[i]->next->game->gameName);
    free(hashtable->entries[i]->next->game);
  }
  free(hashtable->entries[i]->game->gameName);
  free(hashtable->entries[i]->game);
  free(hashtable->entries[i]);
  }
}

int main() {
  char choice;
  int cmd_count = 0;
  ht * hashtable = hash_init();
  ht * game_ht = hash_init();

  while ((choice = getchar()) != 'x' && choice != EOF) {
    switch (choice) {
      case 'a':
        cmd_count++;
        addGame(hashtable, game_ht, cmd_count);
        break;
      case 'p':
        cmd_count++;
        getGame(game_ht, cmd_count);
        break;
      case 'A':
        cmd_count++;
        addTeam(hashtable, cmd_count);
        break;
      case 'P':
        cmd_count++;
        getTeam(hashtable, cmd_count);
        break;
    }
  }
  destroyTeam(hashtable);
  destroyGame(game_ht);
  return 0;
}
