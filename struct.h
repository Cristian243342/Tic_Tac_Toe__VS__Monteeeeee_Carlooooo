#pragma once
#include <stdint.h>

typedef struct list_t {
    void *data;
    struct list_t *prev;
    struct list_t *next;
} list_t;

typedef struct mc_node_t {
    int64_t wins, sims;
    double value;
    int8_t turn, child_nr, max_children; // -1 for 0 and 1 for X; move_nr = nr of children
    list_t *child_list;
    int8_t **image;
    struct mc_node_t *parent;
} mc_node_t;
