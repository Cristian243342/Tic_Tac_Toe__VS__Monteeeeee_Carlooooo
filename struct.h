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
    int64_t wins, sims;  // wins = no. of wins, sims = no. of simulations

    double value;  // maximum selection value of the node in the tree

    //  -1 for O and 1 for X;
    int8_t turn, child_nr, max_children; // (???)

    list_t *child_list;  // list of children used for ????

    int8_t **image;  // current state of the board

    struct mc_node_t *parent;  // parent node
} mc_node_t;