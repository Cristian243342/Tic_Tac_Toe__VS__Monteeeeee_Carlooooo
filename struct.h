/* Copyright Lazar Cristian-Stefan 314CA && Munteanu Eugen 315CA, 2022-2023 */
#pragma once
#include <stdint.h>

// Structure for a singly linked list, used for Monte Carlo tree
typedef struct list_t {
    void *data;
    struct list_t *prev;
    struct list_t *next;
} list_t;

// Structure for a node in the Monte Carlo tree
typedef struct mc_node_t {
    int64_t wins;  // Number of wins.
    
    int64_t sims;  // Number of simulations.

    
    double value;  // Value for selecting the best node for expansion.
    
    int8_t turn;  // Stores whose turn it is in the game states,
                  // stored by the node (1 for X; -1 for O).

    
    int8_t child_nr;  // The number of child game states of the node.

    
    int8_t max_children;  // // The maximum number of children the node can have,
                          // depending on the board state.

    
    list_t *child_list;  // // The list of expansions from the node.

    
    int8_t **image;  //  The state of the board stored in the node
                     //   (1 -> X; 0 -> empty; -1 -> O).
   
    struct mc_node_t *parent;  // The parrent node.

} mc_node_t;