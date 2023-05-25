/* Copyright Lazar Cristian-Stefan 314CA && Munteanu Eugen 315CA, 2022-2023 */
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "struct.h"

#define C sqrt(2)
#define MAX_STR 64

// Clear console on the start of the game, depending on the OS
#ifdef _WIN32
    void clear_console(void)
    {
        system("cls");
    }
#elif defined(__unix__) || defined(__APPLE__)
    void clear_console(void)
    {
        system("clear");
    }
#else
    void clear_console(void)
    {
        // Unsupported operating system
        // Send error message to stderr
        fprintf(stderr, "Unsupported operating system."
                        "Clearing console cannot be done\n");
    }
#endif

/* Returns:
        -1 is 0 wins,
        0 if it's a tie,
        1 if X wins,
        or 2 if the game isn't over yet.
*/
int8_t game_is_over(int8_t **table)
{
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
        if (sum[i] == 3)
            return 1;
        else if (sum[i] == -3)
            return -1;

    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (!table[i][j])
                ver = 1;

    if (ver)
        return 2;

    return 0;
}

// Frees the memory allocated for the table.
void free_tb(int8_t **table)
{
    int i;
    for (i = 0; i < 3; i++) {
        free(table[i]);
        table[i] = NULL;
    }

    free(table);
    table = NULL;
}

// Determines the maximum selection value.
double find_max(mc_node_t *node, uint64_t i)
{
    if (game_is_over(node->image) != 2)
        return -10;

    if (node->wins == -9 && node->sims == 1)
        return -10;
    
    if (node->wins == 9 && node->sims == 1)
        return -10;

    node->value = (double)(node->wins / node->sims) +
                  (double)(C * sqrt(log(i) / node->sims));

    double max, tmp_max;
    max = node->value;
    // Checks if the node can be expanded.
    if (node->max_children == node->child_nr)
        max = -10;

    list_t *curr = node->child_list;
    for (; curr; curr = curr->next) {
        tmp_max = find_max(curr->data, i);
        if (tmp_max > max)
            max = tmp_max;
    }

    return max;
}

// Finds the selected node.
mc_node_t *select_node(mc_node_t *node, double max)
{
    if (game_is_over(node->image) != 2)
        return NULL;

    if (max == node->value && node->max_children != node->child_nr)
        return node;

    mc_node_t *ret;
    list_t *curr = node->child_list;
    for (; curr; curr = curr->next) {
        ret = select_node(curr->data, max);
        if (ret)
            return ret;
    }

    return NULL;
}

// Makes an expedition (aka. makes a random-ish move).
mc_node_t *next_expansion(mc_node_t *node)
{
    // Creates a new node and initialize the correspondent data types
    mc_node_t *new_node = malloc(sizeof(mc_node_t));

    new_node->max_children = node->max_children - 1;
    new_node->value = 0;
    new_node->child_list = NULL;
    new_node->child_nr = 0;
    new_node->parent = node;
    new_node->sims = 0;
    new_node->turn = -node->turn;
    new_node->wins = 0;
    new_node->image = calloc(3, sizeof(int8_t *));

    uint8_t i, j;
    for (i = 0; i < 3; i++) {
        new_node->image[i] = calloc(3, sizeof(int8_t));
        memcpy(new_node->image[i], node->image[i], 3);
    }

    uint8_t count_i = 3, count_j;

    // Temporarely marks all moves that have already been expanded.
    list_t *curr = node->child_list;
    mc_node_t *aux;
    for (; curr; curr = curr->next) {
        aux = curr->data;
        for (i = 0; i < 3; i++)
            for (j = 0; j < 3; j++)
                if (new_node->image[i][j] == 0 && aux->image[i][j] != 0)
                    new_node->image[i][j] = 3;
    }

    int8_t **table = calloc(3, sizeof(int8_t *));
    for (i = 0; i < 3; i++) {
        table[i] = calloc(3, sizeof(int8_t));
        memcpy(table[i], node->image[i], 3);
    }

    /**
     * Heuristic: Checks if there is an imediate winning game state to which to
     * expand, in which case it chooses that game state to expand to.
    */
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            if (new_node->image[i][j] == 0) {
                table[i][j] = node->turn;
                if (game_is_over(table) != 2) {
                    free_tb(new_node->image);
                    new_node->image = table;
                    return new_node;
                }
                table[i][j] = 0;
            }

    // Otherwise, chooses a random game state for the expansion.
    uint8_t r = rand() % 9;
    for (i = r / 3; count_i; i = (i + 1) % 3, count_i--)
        for (j = i == r / 3 ? r % 3 : 0, count_j = 3; count_j;
            j = (j + 1) % 3, count_j--)
            if (new_node->image[i][j] == 0) {
                free_tb(new_node->image);
                new_node->image = table;
                new_node->image[i][j] = node->turn;
                return new_node;
            }

    return NULL;
}

// Simulates a Tic Tac Toe game.
int8_t simulate(mc_node_t *head, int8_t start_turn)
{
    int8_t **table = calloc(3, sizeof(int8_t *)), turn = head->turn;

    for (int8_t i = 0; i < 3; i++) {
        table[i] = calloc(3, sizeof(int8_t));
        memcpy(table[i], head->image[i], 3);
    }

    /**
     * Heuristic 1: At every level of the simulation, checks if the current game
     * state has a winning move for the current player, and always chooses that
     * move instead of a random move.
     * 
     * Heuristic 2: If at the first level of the simulation there is a winning
     * move, the expanded node will be marked as a guaranteed win or guaranteed
     * lost for the AI.
     */
    bool first = true;
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

        first = false;

        // Otherwise, simply chooses a random move.
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

    // Determines the value for the function to return.
    if (game_is_over(table) == -start_turn) {
        if (first)
            turn = 9;
        else
            turn = 1;
    } else if (game_is_over(table) == start_turn) {
        if (first)
            turn = -9;
        else
            turn = -1;
    } else {
        turn = 0;
    }

    for (int i = 0; i < 3; i++) {
        free(table[i]);
        table[i] = NULL;
    }
    free(table);
    table = NULL;

    return turn;
}

// Frees the memory allocated for a tree.
void free_tree(mc_node_t *node)
{
    list_t *child = node->child_list;
    if (child) {
        for (node->child_nr--; node->child_nr > 0; node->child_nr--) {
            child = child->next;

            free_tree(child->prev->data);
            child->prev->data = NULL;
            free(child->prev);
            child->prev = NULL;
        }

        free_tree(child->data);
        child->data = NULL;
        free(child);
        child = NULL;
    }

    free_tb(node->image);
    free(node);
    node = NULL;
}

int main(void)
{
    // Uses current time as seed for the rand() function.
    srand(time(NULL));

    clear_console();  // Clean screen

    printf("Let's play Tic Tac Toe!\n\n");
    printf("What do you want to play as? ");

    // Allocates the head of the tree.
    mc_node_t *tree_head = malloc(sizeof(mc_node_t));
    tree_head->image = calloc(3, sizeof(int8_t *));
    for (int8_t i = 0; i < 3; i++)
        tree_head->image[i] = calloc(3, sizeof(int8_t));

    int8_t start_turn, line, col, rez;

    // Read the player symbol from STDIN (1 -> X, -1 -> O).
    char player_symbol;
    char lime[MAX_STR];

    do {
        scanf("%[^\n]", lime);
        getchar();
        if (sscanf(lime, "%c", &player_symbol) != 1) {
            puts("Invalid symbol. Please try again.");
            continue;
        }
        if (player_symbol == 'X' || player_symbol == 'x') {
            start_turn = 1;
        } else {
            if (player_symbol == 'O' || player_symbol == 'o')
                start_turn = -1;
            else
                printf("Please choose X or O.\n");
        }
    } while (player_symbol != 'X' && player_symbol != 'O' &&
             player_symbol != 'x' && player_symbol != 'o');

    printf("\n");

    uint32_t sims_depth = 0;
    printf("Choose Dificulty (Easy (1), Medium (2), Hard (3)): ");

    // Sets the difficulty of the game, determined by the number of simulations.
    do {
        scanf("%[^\n]", lime); getchar();

        if (sscanf(lime, "%d", &sims_depth) != 1) {
            puts("Invalid dificulty. Please try again.");
            continue;
        }

        switch (sims_depth) {
        case 1: {
            sims_depth = 50;
            break;
        }
        case 2: {
            sims_depth = 600;
            break;
        }
        case 3: {
            sims_depth = 2600;
            break;
        }
        default:
            puts("Invalid dificulty. Please try again.");
            continue;
        }

        break;
    } while (1);

    printf("\nChoose the position for the next symbol\n");
    printf("For example, \"0 0\" is the upper-left corner and\n"
           "\"2 1\" is the bottom-middle position.\n\n");

    // If the player has the starting turn, prompts him to make a move.
    if (start_turn == 1) {
        do {
            scanf("%[^\n]", lime);
            getchar();
            if (sscanf(lime, "%hhd%hhd", &line, &col) != 2) {
                puts("Invalid position. Please try again.");
                continue;
            }
            if (line > 2 || line < 0 || col > 2 || col < 0) {
                puts("Invalid position. Please try again.");
                continue;
            }
            tree_head->image[line][col] = 1;
            break;
        } while (1);

        tree_head->max_children = 8;
    } else {
        tree_head->max_children = 9;
    }

    // Initializes the tree head.
    tree_head->value = 0;
    tree_head->child_nr = 0;
    tree_head->wins = 0;
    tree_head->sims = 0;
    tree_head->turn = -start_turn;
    tree_head->child_list = NULL;
    tree_head->parent = NULL;

    mc_node_t *prev_head = tree_head;

    // Loop until the game is finished.
    double max;
    while (game_is_over(tree_head->image) == 2) {
        printf("\nWait...\n\n");
        mc_node_t *node, *new_node;
        max = -10;
        int8_t i, j;

        for (uint32_t t = 2; t < sims_depth + 2; t++) {
            // Checks if the tree is unpopulated (aka. first move).
            if (!tree_head->child_list) {
                // Checks if the tree head is a terminal node.
                if (game_is_over(tree_head->image) != 2)
                    break;
                node = tree_head;
            } else {
                // Finds the highest value within the tree.
                max = find_max(tree_head, t);

                // Verifies if there is an expandable node
                // (aka. if the tree is full).
                if (max == -10)
                    break;

                // Finds the best node from which to expand.
                node = select_node(tree_head, max);
            }
            // Creates a new node to expand the tree.
            new_node = next_expansion(node);
            node->child_nr++;

            list_t *n_ll_node = malloc(sizeof(list_t));
            // Populates a doubly linked list with the new node and places it at
            // the start of the child list of its parent.
            n_ll_node->data = new_node;
            n_ll_node->next = node->child_list;
            n_ll_node->prev = NULL;

            if (node->child_list)
                node->child_list->prev = n_ll_node;
            node->child_list = n_ll_node;

            // Calculates the result of a random simulation
            // from the board state of the expansion.
            rez = simulate(new_node, start_turn);

            // Backpropagates the result from parent node to parent node,
            // all the way to the tree head.
            for (; new_node; new_node = new_node->parent) {
                new_node->sims += 1;
                new_node->wins += rez;
                if (rez == -9) {
                    new_node->value = -9;
                    rez = -1;
                }
                if (rez == 9) {
                    new_node->value = 9;
                    rez = 1;
                }
            }
        }

        // Find the best move
        // (direct child of the tree root with the highest value).
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

        // After the new move, the whole table will be printed (again).
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

        // The player makes his move.
        do {
            printf("Your turn: ");
            scanf("%[^\n]", lime);
            getchar();
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

        // Make a new tree head for the current board state.
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
        player_move_state->max_children = tree_head->max_children - 1;
        player_move_state->parent = NULL;
        player_move_state->child_list = NULL;

        free_tree(prev_head);
        tree_head = player_move_state;
        prev_head = tree_head;
    }

    printf("\n");
    // After the last move, print the board one more time.
    if (player_symbol == 'X' || player_symbol == 'x') {
        for (int8_t i = 0; i < 3; i++) {
            for (int8_t j = 0; j < 3; j++)
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
            printf("\n");
        }
        printf("\n");
    }

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

    free_tree(prev_head);
    return 0;
}
