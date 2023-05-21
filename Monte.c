#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "struct.h"

#define SIMS_DEPTH 2000
#define C sqrt(2)
#define MAX_STR 20

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
mc_node_t *next_expansion(mc_node_t *node)
{
	// Creates a new node and initialize the correspondent data types
	mc_node_t *new_node = malloc(sizeof(mc_node_t));

	new_node->max_children = 0;
	new_node->value = 0;
	new_node->child_list = NULL;
	new_node->child_nr = 0;
	new_node->parent = node;
	new_node->sims = 0;
	new_node->turn = -node->turn;
	new_node->wins = 0;
	new_node->image = calloc(3, sizeof(int8_t *));

	for (int i = 0; i < 3; i++) {
		new_node->image[i] = calloc(3, sizeof(int8_t));
		memcpy(new_node->image[i], node->image[i], 3);
	}

	uint8_t i, j, count_i = 3, count_j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (new_node->image[i][j] == 0) {
				new_node->image[i][j] = node->turn;
				if (game_is_over(new_node->image) != 2)
					return new_node;
				new_node->image[i][j] = 0;
			}
		}
	}

	uint8_t r = rand() % 9;
	for (i = r / 3; count_i; i = (i + 1) % 3, count_i--)
		for (j = i == r / 3 ? r % 3 : 0, count_j = 3; count_j; j = (j + 1) % 3,
			 count_j--)
			if (new_node->image[i][j] == 0) {
				new_node->image[i][j] = node->turn;
				return new_node;
			}

	return NULL;
}

// Simulates a Tic Tac Toe game
int8_t simulate(mc_node_t *head, int8_t start_turn)
{
	int8_t **table = calloc(3, sizeof(int8_t *)), turn = head->turn;

	for (int8_t i = 0; i < 3; i++) {
		table[i] = calloc(3, sizeof(int8_t));
		memcpy(table[i], head->image[i], 3);
	}

	while (game_is_over(table) == 2) {
		int8_t i, j;
		bool ver = false;
			for (i = 0; i < 3; i++) {
				for (j = 0; j < 3; j++) {
					if (table[i][j] == 0) {
						table[i][j] = turn;
						if (game_is_over(table) != 2) {
							ver = true;
							break;
						}
						table[i][j] = 0;
					}
				}
				if (ver)
					break;
			}
		
		if (ver)
			break;

		int8_t r = rand() % 9;
		for (i = r / 3; i != ((int)(r / 3) + 2) % 3; i = (i + 1) % 3) {
			for (j = i == r / 3 ? r % 3 : 0; j < 3; j++)
				if (table[i][j] == 0) {
					table[i][j] = turn;
					r = -1;
					break;
				}

			if (r == -1)
				break;
		}

		if (r != -1) {
			for (j = 0; j < 3; j++)
				if (table[i][j] == 0) {
					table[i][j] = turn;
					r = -1;
					break;
				}
		}

		turn *= -1;
	}

	if (game_is_over(table) == -start_turn)
		turn = 1;
	else if (game_is_over(table) == start_turn)
		turn = -2;
	else
		turn = -1;

	for (int i = 0; i < 3; i++) {
		free(table[i]);
		table[i] = NULL;
	}
	free(table);
	table = NULL;

	return turn;
}

void free_tree(mc_node_t *node) {
  list_t *child = node->child_list;
  if (child) {
    for (node->child_nr--; node->child_nr > 0; node->child_nr--) {
      child = child->next;
      free_tree(child->prev->data);
      free(child->prev);
    }
    free_tree(child->data);
    free(child);
  }

  int i = 0;
  for (; i < 3; i++) free(node->image[i]);
  free(node->image);
  free(node);
}

int main(void)
{
	// Uses current time as seed for the rand() function
	srand(time(NULL));

	system("clear");  // Clean console (Linux)

	printf("Let's play Tic Tac Toe!\n\n");
	printf("What do you want to play as? ");
	// Allocate the head of the tree
	mc_node_t *tree_head = malloc(sizeof(mc_node_t));
	tree_head->image = calloc(3, sizeof(int8_t *));
	for (int8_t i = 0; i < 3; i++)
		tree_head->image[i] = calloc(3, sizeof(int8_t));

	int8_t start_turn, line, col, rez;

	// Read the player symbol from STDIN (1 -> X, -1 -> O)
	char player_symbol;
	char lime[MAX_STR];  // will be used to read a line, where is necessary

	do {
		scanf("%[^\n]", lime); getchar();
		if (sscanf(lime, "%c", &player_symbol) != 1) {
			puts("Invalid symbol. Please try again.");
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

	printf("Choose the position for %c:\n", player_symbol);
	printf("For example, \"0 0\" is the upper-left corner and\n"
		   "\"2 1\" is the bottom-middle position.\n");

  // If the player is the first to start the game
	if (start_turn == 1) {
		do {
			scanf("%[^\n]", lime); getchar();
			if (sscanf(lime, "%hhd%hhd", &line, &col) != 2) {
				puts("Invalid position. Please try again.");
				continue;
			}
			if (line > 2 || line < 0 || col > 2 || col < 0) {
				puts("Invalid position. Please try again.");
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
	// Loop until the game is finished
	while (game_is_over(tree_head->image) == 2) {
		printf("\nWait for AI's turn...\n");
		mc_node_t *node, *new_node;
		double max = -10;
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

				// Verifies if there is an expandable node
				// (aka. if the tree is full)
				if (max == -10)
					break;
				node = select_node(tree_head, max);
			}
			// Creates a new node to expand the tree.
			new_node = next_expansion(node);
			node->child_nr += 1;

			// Find the maximum number of children the new node can have.
			for (i = 0; i < 3; i++)
				for (j = 0; j < 3; j++)
					if (!new_node->image[i][j])
						new_node->max_children++;

			list_t *n_ll_node = malloc(sizeof(list_t));
			// Populates a list_t with the new node and places it at
			// the start of the child list of its parent
			n_ll_node->data = new_node;
			n_ll_node->next = node->child_list;
			n_ll_node->prev = NULL;

			if (node->child_list)
				node->child_list->prev = n_ll_node;
			node->child_list = n_ll_node;

			// Calculates the result of a random simulation
			// from this board state
			rez = simulate(new_node, start_turn);

			// Backpropagates the result to all the parent nodes,
			// all the way to the tree head
			for (; new_node; new_node = new_node->parent) {
				new_node->sims += 1;
				new_node->wins += rez;
			}
		}

		// Find the best move
		list_t *curr = tree_head->child_list;
		max = -10;
		for (; curr; curr = curr->next) {
			mc_node_t *curr_data = ((mc_node_t *)curr->data);
			curr_data->value = (double)(curr_data->wins / curr_data->sims);
			if (max < curr_data->value)
				max = curr_data->value;
		}

		curr = tree_head->child_list;
		for (; curr; curr = curr->next)
			if (((mc_node_t *)curr->data)->value == max)
				break;

		tree_head = curr->data;

		// After the new move, the whole table will be printed (again)
		printf("\n");
		for (int8_t i = 0; i < 3; i++) {
			for (int8_t j = 0; j < 3; j++) {
				switch (tree_head->image[i][j]) {
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

		if (game_is_over(tree_head->image) != 2)
			break;

		// The player makes his move
		do {
			scanf("%[^\n]", lime); getchar();
			if (sscanf(lime, "%hhd%hhd", &line, &col) != 2) {
				puts("Invalid position. Please try again.");
				continue;
			}
			if (line > 2 || line < 0 || col > 2 || col < 0) {
				puts("Invalid position. Please try again.");
				continue;
			}
			if (tree_head->image[line][col] != 0) {
				puts("Invalid position. Please try again.");
				continue;
			}

			break;
		} while (1);

		mc_node_t *player_move_state = malloc(sizeof(mc_node_t));
		player_move_state->image = malloc(3 * sizeof(int8_t *));

		for (i = 0; i < 3; i++) {
			player_move_state->image[i] = malloc(3 * sizeof(int8_t));
			for (j = 0; j < 3; j++)
				player_move_state->image[i][j] = tree_head->image[i][j];
		}

		player_move_state->image[line][col] = start_turn;
		player_move_state->turn = -start_turn;
		player_move_state->child_nr = 0;
		player_move_state->wins = 0;
		player_move_state->sims = 0;
		player_move_state->max_children = 9;
		player_move_state->parent = NULL;
		player_move_state->child_list = NULL;
		player_move_state->max_children--;
		tree_head->child_nr++;

		list_t *n_l = malloc(sizeof(list_t));
		n_l->data = player_move_state;
		n_l->prev = NULL;
		n_l->next = tree_head->child_list;
		if (tree_head->child_list)
			tree_head->child_list->prev = n_l;

		tree_head->child_list = n_l;
		tree_head = player_move_state;
	
	}

	// After the last move, print the board one more time
	for (int8_t i = 0; i < 3; i++) {
		for (int8_t j = 0; j < 3; j++) {
			switch (tree_head->image[i][j]) {
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

	switch (game_is_over(tree_head->image)) {
	case 1: {
		if (player_symbol == 'X')
			printf("You won! Congratulations!\n\n");
		else
			printf("AI won! Better luck next time...\n\n");
		break;
	}
	case -1: {
		if (player_symbol == 'X')
			printf("AI won! Better luck next time...\n\n");
		else
			printf("You won! Congratulations!\n\n");
		break;
	}
	case 0: {
		printf("It's a draw! You should give it another go...\n\n");
		break;
	}
	default: {
		printf("Failed to determine winner. Check the code...\n\n");
		break;
	}
	}

	free_tree(true_head);
	return 0;
}
