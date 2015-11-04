//
// Created by SG0220005 on 10/24/2015.
//

#ifndef WIREWORLD_WIREWORD_H
#define WIREWORLD_WIREWORD_H

#include "ppm.h"

int wireworld(Grid *grid, int steps, void (*present_simulation)(Grid *));

typedef struct {
    int rank;
    int world_size;
    int my_rows;
    int my_y_offset;
} Node_config;

Node_config get_node(Grid *grid);

#endif //WIREWORLD_WIREWORD_H
