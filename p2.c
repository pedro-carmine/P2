#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 1023
#define TABLE_SIZE 1000

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

typedef struct ht {
  entry **entries;
} ht;

typedef struct orderedGames {
  Game * game;
  struct orderedGames *next;
} orderedGames;


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

int searchGame(entry * entrie, char * buffer) {
  entry * aux = entrie;
  if (aux == NULL) {
    return 0;
  }

  if (aux->game != NULL) {
    while (strcmp(aux->game->gameName, buffer) != 0) {
      if (aux->next == NULL) {
        return 0;
      }
      aux = aux->next;
    }
    return 1;
  }
  else
    return 0;
}


void getGame(ht * game_ht, int cmd_count) {
  char name[MAX_CHAR];
  entry * entrie;
  int key;
  scanf(" %[^\n]", name);


  key = hash(name);
  entrie = game_ht->entries[key];

  if (!searchGame(entrie, name)) {
    printf("%d Jogo inexistente.\n", cmd_count);
  }
  else {
    while (entrie->game != NULL && strcmp(entrie->game->gameName, name) != 0) {
      entrie = entrie->next;
    }
      printf("%d %s %s %s %d %d\n", cmd_count, entrie->game->gameName, entrie->game->t1->nome, entrie->game->t2->nome, entrie->game->score1, entrie->game->score2);
  }
}

Game * addGame(ht * hashtable, ht * game_ht, int cmd_count, int * game_count) {
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
      return NULL;
    }
    game_ht->entries[key] = ht_pair_game(key, gameName, t1, t2, score1, score2);
    (*game_count)++;
    return game_ht->entries[key]->game;
  }

  while (entrie != NULL) {
    if (strcmp(entrie->game->gameName, gameName) != 0 && entrie->next == NULL && entrie->game != NULL && t1 != NULL && t2 != NULL) {
      entrie->next = ht_pair_game(key, gameName, t1, t2, score1, score2);
      (*game_count)++;
      return entrie->next->game;
    }
    else if (strcmp(entrie->game->gameName, gameName) == 0) {
      printf("%d Jogo existente.\n", cmd_count);
      return NULL;
    }
    prev = entrie;
    entrie = prev->next;
  }

  if (t1 == NULL || t2 == NULL) {
    printf("%d Equipa inexistente.\n", cmd_count);
    return NULL;
  }
  prev->next = ht_pair_game(key, gameName, t1, t2, score1, score2);
  (*game_count)++;
  return prev->next->game;
}

void changeScore(ht * game_ht, int cmd_count) {
  char buffer[MAX_CHAR];
  entry * entrie;
  int key;
  int score1, score2;
  scanf(" %[^:]:%d:%d", buffer, &score1, &score2);

  key = hash(buffer);
  entrie = game_ht->entries[key];

  if (!searchGame(entrie, buffer)) {
    printf("%d Jogo inexistente.\n", cmd_count);
    return;
  }

  while (strcmp(entrie->game->gameName, buffer) != 0) {
    entrie = entrie->next;
  }

  if (entrie->game->score1 > entrie->game->score2) {
    entrie->game->t1->wins--;
  }
  else if (entrie->game->score2 > entrie->game->score1) {
    entrie->game->t2->wins--;
  }

  if (score1 > score2) {
    entrie->game->t1->wins++;
  }
  else if (score2 > score1) {
    entrie->game->t2->wins++;
  }

  entrie->game->score1 = score1;
  entrie->game->score2 = score2;
}

entry * destroyTeamEntry(entry * head) {
  entry * new = head;
  while (new != NULL) {
    entry * aux = new->next;
    free(new->t->nome);
    free(new->t);
    free(new);
    new = aux;
  }
  return NULL;
}

entry * destroyGameEntry(entry * head) {
  entry * new = head;
  while (new != NULL) {
    entry * aux = new->next;
    free(new->game->gameName);
    free(new->game);
    free(new);
    new = aux;
  }
  return NULL;
}

void destroyTeam(ht * hashtable) {
  int i;
  for (i = 0; i < TABLE_SIZE; i++) {
    if (hashtable->entries[i] == NULL) {
      continue;
    }
    else {
      destroyTeamEntry(hashtable->entries[i]);
    }
  }
  free(hashtable->entries);
  free(hashtable);
}

orderedGames * removeGame(ht * game_ht, int cmd_count, orderedGames * head) {
  char buffer[MAX_CHAR];
  entry * entrie;
  entry * prev;
  int key, idx;
  idx = 0;

  scanf(" %[^\n]", buffer);

  key = hash(buffer);
  entrie = game_ht->entries[key];
  /* if theres nothing in the entrie the game doesn't exist */
  if (entrie == NULL) {
    printf ("%d Jogo inexistente.\n", cmd_count);
    return head;
  }

  if (searchGame(entrie, buffer)) {
    /* removes the game from the ordered games list*/
    orderedGames * aux, prevv;
    for (aux = head, prevv = NULL; aux != NULL; prevv = aux, aux = aux->next) {
      if (strcmp(aux->game->gameName, buffer) == 0) {
        if (aux == head) {
          head = aux->next;
        }
        else {
          prevv->next = aux->next;
        }
        free(aux);
        break;
      }
    }
    /* removes the game from the hashtable */
    while (entrie != NULL) {
      /* cycle true the entrie in case theres collision */
      if (strcmp(entrie->game->gameName, buffer) == 0) {

        if (entrie->next == NULL && idx == 0) {
          game_ht->entries[key] = NULL;
        }

        if (entrie->next != NULL && idx == 0) {
          game_ht->entries[key] = entrie->next;
        }

        if (entrie->next == NULL && idx != 0) {
          prev->next = NULL;
        }

        if (entrie->next != NULL && idx != 0) {
          prev->next = entrie->next;
        }

        if (entrie->game->score1 > entrie->game->score2) {
            entrie->game->t1->wins--;
        }

        else if (entrie->game->score2 > entrie->game->score1) {
          entrie->game->t2->wins--;
        }

        free(entrie->game->gameName);
        free(entrie->game);
        free(entrie);
        return head;
      }

      prev = entrie;
      entrie = prev->next;

      ++idx;

    }
  }
  else {
    printf("%d Jogo inexistente.\n", cmd_count);
    return head;
  }
}

int cmp_win(Team * t1, Team * t2) {
  return (t1->wins - t2->wins);
}

/*void sortGames(ht * game_ht, int cmd_count, int game_count) {
  entry * aux;
  int i = 0;
  for (; i < TABLE_SIZE; i++) {
    aux = hashtable->entries[i];
    while (aux->next != NULL) {
      qsort(hashtable, game_count, sizeof(Team), cmp_win(aux->));
    }
  }
}*/

void destroyGame(ht * hashtable) {
  int i;
  for (i = 0; i < TABLE_SIZE; i++) {
    if (hashtable->entries[i] == NULL) {
      continue;
    }
    else {
      destroyGameEntry(hashtable->entries[i]);
    }
  }
  free(hashtable->entries);
  free(hashtable);
}

orderedGames * push(orderedGames * head, Game * game) {
  /* if the game returned from the addGame function is NULL i.e wrong addGame() input, it doesn't add to the ordered list and return the unchanged list */
  if (game == NULL) {
    return head;
  }
  /* if it is a valid game, adds to the end of the list */
  else {
    orderedGames * novo;
    orderedGames * aux;
    novo = malloc(sizeof(orderedGames));
    aux = head;
    /* if the list is not empty */
    if (aux != NULL) {
      while (aux->next != NULL) {
        aux = aux->next;
      }
    }
    /* if the list is empty */
    else {
      novo->game = game;
      novo->next = NULL;
      return novo;
    }
    aux->next = novo;
    novo->game = game;
    novo->next = NULL;
    return head;
  }
}

/* function that frees all orderedGames nodes in the end of the program so theres no memory leak */
orderedGames * destroy(orderedGames * head) {
  orderedGames * novo = head;
  /* cycle until it reaches the end of the list */
  while (novo != NULL) {
    orderedGames * aux = novo->next;
    free(novo);
    novo = aux;
  }
  return NULL;
}

void print(orderedGames * head, int cmd_count) {
  orderedGames * aux;
  for (aux = head; aux != NULL; aux = aux->next) {
    if (aux->game != NULL && aux->game->gameName != NULL) {
    printf("%d %s %s %s %d %d\n", cmd_count, aux->game->gameName, aux->game->t1->nome, aux->game->t2->nome, aux->game->score1, aux->game->score2);
  }
  }
}

int main() {
  orderedGames * init;
  char choice;
  int cmd_count = 0;
  int game_count = 0;
  ht * hashtable = hash_init();
  ht * game_ht = hash_init();
  init = NULL;


  while ((choice = getchar()) != 'x' && choice != EOF) {
    switch (choice) {
      case 'a':
        cmd_count++;
        init = push(init, addGame(hashtable, game_ht, cmd_count, &game_count));
        break;
      case 'p':
        cmd_count++;
        getGame(game_ht, cmd_count);
        break;
      case 'l':
        cmd_count++;
        print(init, cmd_count);
        break;
      case 'A':
        cmd_count++;
        addTeam(hashtable, cmd_count);
        break;
      case 'P':
        cmd_count++;
        getTeam(hashtable, cmd_count);
        break;
      case 's':
        cmd_count++;
        changeScore(game_ht, cmd_count);
        break;
      case 'r':
        cmd_count++;
        init = removeGame(game_ht, cmd_count, init);
        break;
      case 'g':
        cmd_count++;
      /*  sortGames(init, cmd_count, team_count);*/
        break;
    }
  }

  destroy(init);
  destroyGame(game_ht);
  destroyTeam(hashtable);
  return 0;
}
