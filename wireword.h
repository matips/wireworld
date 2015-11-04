//
// Created by SG0220005 on 10/24/2015.
//

#ifndef WIREWORLD_WIREWORD_H
#define WIREWORLD_WIREWORD_H

typedef enum {
    empty = 0,
    head = 1,
    tail = 2,
    conductor = 3
} State;

typedef struct {
    int x, y;
    State *data;
} Grid;

int wireworld(Grid *grid, int steps, void (*present_simulation)(Grid *));

typedef struct {
    int rank;
    int world_size;
    int my_rows;
    int my_y_offset;
} Node_config;

Node_config get_node(Grid *grid);


#endif //WIREWORLD_WIREWORD_H
