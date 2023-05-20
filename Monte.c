#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "struct.h"

#define SIMS_DEPTH 100
#define C sqrt(2)
#define MAX_STR 128

// Returns -1 is 0 wins, 0 if it's a tie, 1 if X wins, or 2 if the game isn't
// over
int8_t game_is_over(int8_t **table) {
  int8_t sum[8] = {0}, i, j, ver = 0;
  for (i = 0; i < 3; i++) {
    sum[0] += table[i][i];
    sum[1] += table[i][2 - i];
    sum[2] += table[0][i];
    sum[3] += table[1][i];
    sum[4] += table[2][i];
    sum[5] += table[i][0];
    sum[6] += table[i][1];
    sum[7] += table[i][2];
  }
  for (i = 0; i < 8; i++)
    if (sum[i] == 3) {
      return 1;
    } else if (sum[i] == -3) {
      return -1;
    }

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      if (!table[i][j]) ver = 1;

  if (ver) return 2;
  return 0;
}

mc_node_t *next_expansion(mc_node_t *node);

// Clears the node of temporary "3" values;
void clean_node(mc_node_t *node) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (node->image[i][j] == 3) node->image[i][j] = 0;
}

// Determines the maximum selection value.
double find_max(mc_node_t *node, uint64_t i) {
  if (game_is_over(node->image) != 2) return -10;
  double max, tmp_max;
  node->value = (double)node->wins / node->sims + C * sqrt(log(i) / node->sims);
  max = node->value;
  // Checks if the node can be expanded.
  if (node->max_children == node->child_nr) max = -10;

  list_t *curr = node->child_list;
  for (; curr; curr = curr->next) {
    tmp_max = find_max(curr->data, i);
    if (tmp_max > max) max = tmp_max;
  }
  return max;
}

// Finds the selected node.
mc_node_t *select_node(mc_node_t *node, double max) {
  if (game_is_over(node->image) != 2) return NULL;
  if (max == node->value && node->max_children != node->child_nr) return node;
  mc_node_t *ret;
  list_t *curr = node->child_list;
  for (; curr; curr = curr->next) {
    ret = select_node(curr->data, max);
    if (ret) return ret;
  }

  return NULL;
}

// Makes an expedition (aka. makes a random move)
mc_node_t *next_expansion(mc_node_t *node) {
  // Creates a new node
  mc_node_t *new_node = malloc(sizeof(mc_node_t));
  int8_t i, tmp = node->child_nr;

  new_node->max_children = 0;
  new_node->value = 0;
  new_node->child_list = NULL;
  new_node->child_nr = 0;
  new_node->parent = node;
  new_node->sims = 0;
  new_node->turn = -node->turn;
  new_node->wins = 0;
  new_node->image = calloc(3, sizeof(int8_t *));
  for (i = 0; i < 3; i++) {
    new_node->image[i] = calloc(3, sizeof(int8_t));
    memcpy(new_node->image[i], node->image[i], 3);
  }

  // Finds a new expansion.
  int8_t nr_tang = 0;
  if (!new_node->image[1][1]) {
    for (i = 0; i < 3; i += 2) {
      if (new_node->image[i][1] == node->turn) nr_tang++;
      if (new_node->image[1][i] == node->turn) nr_tang++;
    }
    if (nr_tang >= 2) {
      if (!tmp) {
        new_node->image[1][1] = node->turn;
        return new_node;
      }
      new_node->image[1][1] = 3;
      tmp--;
    }
  }

  for (i = 0; i < 3; i += 2) {
    // Checks if the is a more valuable h/b margin position.
    nr_tang = 0;
    if (!new_node->image[i][1]) {
      if (new_node->image[i][0] == node->turn) nr_tang++;
      if (new_node->image[i][2] == node->turn) nr_tang++;
      if (new_node->image[1][1] == node->turn) nr_tang++;
      if (nr_tang >= 2) {
        if (!tmp) {
          new_node->image[i][1] = node->turn;
          return new_node;
        }
        new_node->image[i][1] = 3;
        tmp--;
      }
    }

    // Checks if there is a more valuable side placement.
    nr_tang = 0;
    if (!new_node->image[1][i]) {
      if (new_node->image[0][i] == node->turn) nr_tang++;
      if (new_node->image[2][i] == node->turn) nr_tang++;
      if (new_node->image[1][1] == node->turn) nr_tang++;
      if (nr_tang >= 2) {
        if (!tmp) {
          new_node->image[1][i] = node->turn;
          return new_node;
        }
        new_node->image[1][i] = 3;
        tmp--;
      }
    }
  }

  for (i = 0; i < 3; i += 2) {
    // Checks it there is a more valuable top corner node.
    nr_tang = 0;
    if (!new_node->image[0][i]) {
      if (new_node->image[1][i] == node->turn) nr_tang++;
      if (new_node->image[0][1] == node->turn) nr_tang++;
      if (nr_tang >= 1) {
        if (!tmp) {
          new_node->image[0][i] = node->turn;
          return new_node;
        }
        new_node->image[0][i] = 3;
        tmp--;
      }
    }

    // Checks if there is a more valuable bottom corner node.
    nr_tang = 0;
    if (!new_node->image[2][i]) {
      if (new_node->image[1][i] == node->turn) nr_tang++;
      if (new_node->image[2][1] == node->turn) nr_tang++;
      if (nr_tang >= 1) {
        if (!tmp) {
          new_node->image[2][i] = node->turn;
          return new_node;
        }
        new_node->image[2][i] = 3;
        tmp--;
      }
    }
  }

  if (!new_node->image[1][1]) {
    if (!tmp) {
      new_node->image[1][1] = node->turn;
      return new_node;
    }
    tmp--;
  }

  for (i = 0; i < 3; i += 2) {
    if (!new_node->image[i][1]) {
      if (!tmp) {
        new_node->image[i][1] = node->turn;
        return new_node;
      }
      tmp--;
    }

    if (!new_node->image[1][i]) {
      if (!tmp) {
        new_node->image[1][i] = node->turn;
        return new_node;
      }
      tmp--;
    }
  }

  for (i = 0; i < 3; i += 2) {
    if (!new_node->image[0][i]) {
      if (!tmp) {
        new_node->image[0][i] = node->turn;
        return new_node;
      }
      tmp--;
    }

    if (!new_node->image[2][i]) {
      if (!tmp) {
        new_node->image[2][i] = node->turn;
        return new_node;
      }
      tmp--;
    }
  }

  return NULL;
}

// Simulates a game.
int8_t simulate(mc_node_t *head, int8_t start_turn) {
  int8_t **table = calloc(3, sizeof(int8_t *)), turn = head->turn;
  for (int8_t i = 0; i < 3; i++) {
    table[i] = calloc(3, sizeof(int8_t));
    memcpy(table[i], head->image[i], 3);
  }

  while (game_is_over(table) == 2) {
    int8_t r = rand() % 9, i, j;
    for (i = r / 3; i != ((int)(r / 3) + 2) % 3; i = (i + 1) % 3) {
      for (j = i == r / 3 ? r % 3 : 0; j < 3; j++)
        if (table[i][j] == 0) {
          table[i][j] = turn;
          r = -1;
          break;
        }
      if (r == -1) break;
    }

    if (r != -1)
      for (j = 0; j < 3; j++)
        if (table[i][j] == 0) {
          table[i][j] = turn;
          r = -1;
          break;
        }

    turn *= -1;
  }

  if (game_is_over(table) == -start_turn)
    turn = 1;
  else if (game_is_over(table) == start_turn)
    turn = -1;
  else
    turn = 0;

  for (int8_t i = 0; i < 3; i++) free(table[i]);
  free(table);
  return turn;
}

void free_tree(mc_node_t *node) {
  list_t *child = node->child_list;
  if (child)
    for (; child->next; child = child->next) {
      free_tree(child->data);
      child->data = NULL;
      if (child->prev) {
        free(child->prev);
        child->prev = NULL;
      }
    }

  if (child) {
    if (child->prev) {
      free(child->prev);
      child->prev = NULL;
    }
    free_tree(child->data);
    child->data = NULL;
    free(child);
    child = NULL;
  }

  int i = 0;
  for (; i < 3; i++) {
    free(node->image[i]);
    node->image[i] = NULL;
  }
  free(node->image);
  node->image = NULL;
  free(node);
  node = NULL;
}

int main(void) {

  system("clear");  // Clean console (Linux)

  printf("Let's play Tic Tac Toe!\n\n");
  printf("What do you want to play as? ");

  // Uses current time a seed for the rand() function
  srand(time(NULL));

  // Allocate the head of the tree
  mc_node_t *tree_head = malloc(sizeof(mc_node_t));
  tree_head->image = calloc(3, sizeof(int8_t *));
  for (int8_t i = 0; i < 3; i++)
    tree_head->image[i] = calloc(3, sizeof(int8_t));

  int8_t start_turn, line, col, rez;

  // Read the player symbol from STDIN
  char player_symbol;
  char lime[MAX_STR]; // will be used to read a line, where is necessary
  do {
    scanf("%[^\n]", lime); getchar();
    if (sscanf(lime, "%c", &player_symbol) != 1) {
      puts("Invalid position. Please try again. (1)");
      continue;
    }
    if (player_symbol == 'X') {
      start_turn = 1;
    } else {
        if (player_symbol == 'O')
          start_turn = -1;
        else
          printf("Please choose X or O.\n");
    }
  } while (player_symbol != 'X' && player_symbol != 'O');
  printf("\n");

  printf("Choose the position for %c:\n", player_symbol); // 1-player, 1-ai
  printf("For example, \"0 0\" is the upper-left corner and\n"
    "\"2 2\" is the bottom-right corner.\n");

  // If the player is the first to start the game
  if (start_turn == 1) {
    do {
      scanf("%[^\n]", lime); getchar();
      if (sscanf(lime, "%hhd%hhd", &line, &col) != 2) {
        puts("Invalid position. Please try again. (1)");
        continue;
      }
      if (line > 2 || line < 0 || col > 2 || col < 0) {
        puts("Invalid position. Please try again. (2)");
        continue;
      }
      tree_head->image[line][col] = 1;  // 1->X; 0->empty; -1->0
      break;
    } while (1);
    tree_head->max_children = 8;
  } else {
    tree_head->max_children = 9;
  }

  // Initialize tree head.
  tree_head->value = 0;
  tree_head->child_nr = 0;
  tree_head->wins = 0;
  tree_head->sims = 0;
  tree_head->turn = -start_turn;  // -1->0; 1->X
  tree_head->child_list = NULL;
  tree_head->parent = NULL;

  mc_node_t *true_head = tree_head;
  // Loop untill the game is finished
  while (game_is_over(tree_head->image) == 2) {
    mc_node_t *node, *new_node;
    double max = 0;
    int8_t i, j;
    for (uint32_t t = 2; t < SIMS_DEPTH + 2; t++) {
      // Checks if the tree is unpopulated (aka. first move)
      if (!tree_head->child_list) {
        // Checks if the tree head is a terminal node
        // if (game_is_over(tree_head->image) != 2) break;
        node = tree_head;
      } else {
        // Selects the best node from which to expand
        max = find_max(tree_head, t);
        // Verifies if there is an expandable node (aka. if the tree is full)
        if (max == -10) break;
        node = select_node(tree_head, max);
      }

      // Creates a new node to expand the tree

      // NULL node gets input in function
      new_node = next_expansion(node);
      node->child_nr += 1;
      clean_node(new_node);
      // Find the maximum number of children the new node can have.
      for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
          if (!new_node->image[i][j]) new_node->max_children++;

      list_t *n_ll_node; n_ll_node = malloc(sizeof(list_t));
      // Populates a list_t with the new node and places it at the start of the
      // child list
      n_ll_node->data = new_node;
      n_ll_node->next = node->child_list;
      n_ll_node->prev = NULL;
      node->child_list = n_ll_node;

      // Calculates the result of a random simulation from this board state
      rez = simulate(new_node, start_turn);

      // Backpropagates the result to all the parent nodes, all the way to the
      // tree head
      for (; new_node != NULL; new_node = new_node->parent) {
        new_node->sims += 1;
        new_node->wins += rez;
      }
    }

    // Find the best move
    list_t *curr = tree_head->child_list;
    max = -10;
    for (; curr; curr = curr->next) {
      ((mc_node_t *)curr->data)->value =
          (double)((mc_node_t *)curr->data)->wins /
          ((mc_node_t *)curr->data)->sims;
      if (max < ((mc_node_t *)curr->data)->value)
        max = ((mc_node_t *)curr->data)->value;
    }

    curr = tree_head->child_list;
    for (; curr; curr = curr->next)
      if (((mc_node_t *)curr->data)->value == max) break;

    tree_head = curr->data;

    // After the new move, the whole table will be printed (again)
    for (int8_t i = 0; i < 3; i++) {
      for (int8_t j = 0; j < 3; j++) {
        switch(tree_head->image[i][j]) {
        case 1: {
          printf("%2c", 'X');
          break;
        }
        case -1: {
          printf("%2c", 'O');
          break;
        }
        default: {
          printf("%2c", '-');
          break;
        }
        }
      }
      printf("\n");
    }
    printf("\n");
  
    if (game_is_over(tree_head->image) != 2) break;

    // The player makes his move
    do {
      scanf("%[^\n]", lime); getchar();
      if (sscanf(lime, "%hhd%hhd", &line, &col) != 2) {
        puts("Invalid position. Please try again. (3)");
        continue;
      }
      if (line > 2 || line < 0 || col > 2 || col < 0) {
        puts("Invalid position. Please try again. (4)");
        continue;
      }
      if (tree_head->image[line][col] != 0) {
        puts("Invalid position. Please try again. (5)");
        continue;
      }
      tree_head->image[line][col] = start_turn;
      tree_head->turn *= -1;
      tree_head->child_nr = 0;
      tree_head->wins = 0;
      tree_head->sims = 0;
      tree_head->parent = NULL;
      tree_head->max_children--;
      break;
  
    } while(1);

    // Looking for an existing node for the current table, after the move
    int8_t ver = 0;
    for (curr = tree_head->child_list; curr; curr = curr->next) {
      ver = 1;
      for (int8_t i = 0; i < 3; i++)
        for (int8_t j = 0; j < 3; j++)
          if (tree_head->image[i][j] != ((mc_node_t *)curr->data)->image[i][j])
            ver = 0;
      if (ver) break;
    }

    // If ver is 1, than the node for the current state already exists
    if (ver) {
      tree_head = curr->data;
      tree_head->parent = NULL;
    } else {
      curr = tree_head->child_list;
      if (curr)
        for (; curr->next; curr = curr->next) {
          free_tree(curr->data);
          curr->data = NULL;
          if (curr->prev) {
            free(curr->prev);
            curr->prev = NULL;
          }
        }

      if (curr) {
        if (curr->prev) {
          free(curr->prev);
          curr->prev = NULL;
        }
        free_tree(curr->data);
        curr->data = NULL;
        free(curr);
        curr = NULL;
      }
      tree_head->child_list = NULL;
    }
  }

  // After the last move, print the board one more time
  for (int8_t i = 0; i < 3; i++) {
      for (int8_t j = 0; j < 3; j++) {
        switch(tree_head->image[i][j]) {
        case 1: {
          printf("%2c", 'X');
          break;
        }
        case -1: {
          printf("%2c", 'O');
          break;
        }
        default: {
          printf("%2c", '-');
          break;
        }
        }
      }
      printf("\n");
    }
    printf("\n");
  
  switch(game_is_over(tree_head->image)) {
  case 1: {
    if (player_symbol == 'X')
      printf("You won! Congratulations! (ez strategy, right?)\n");
    else
      printf("AI won! Better LUCK next time (try decreasing the depth :) )\n");
    break;
  }
  case -1: {
    if (player_symbol == 'X')
      printf("AI won! Better LUCK next time (try decreasing the depth :) )\n");
    else
      printf("You won! Congratulations! (ez strategy, right?)\n");
    break;
  }
  case 0: {
    printf("It's a draw! Most likely to happen, I guess...?\n");
    break;
  }
  default: {
    printf("Failed to determine winner. Check the code...\n");
    break;
  }
  }

  free_tree(true_head);
  return 0;
}