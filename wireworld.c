#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include <omp.h>
#include <unistd.h>
#include <string.h>
#include "simulation.h"
#include "wireword.h"

signed long long index(signed long long x, signed long long y, signed long long x_len) {
    return y * x_len + x;
}

Node_config get_node(Grid *grid) {
    Node_config config;
    MPI_Comm_rank(MPI_COMM_WORLD, &config.rank);
    MPI_Comm_size(MPI_COMM_WORLD, &config.world_size);


    /* Determine my part of the matrix */
    config.my_rows = grid->y / config.world_size + ((config.rank < (grid->y % config.world_size)) ? 1 : 0);
    config.my_y_offset = config.rank * (grid->y / config.world_size);
    if (config.rank > (grid->y % config.world_size)) config.my_y_offset += (grid->y % config.world_size);
    else config.my_y_offset += config.rank;
    return config;
}

int wireworld(Grid *grid, int steps, void (*present_simulation)(Grid *)) {

    Node_config node = get_node(grid);
    MPI_Request req[4];
    MPI_Status status;

    int prev, next;
    /* Set neighbors */
    if (node.rank == 0)
        prev = MPI_PROC_NULL;
    else
        prev = node.rank - 1;
    if (node.rank == (node.world_size - 1))
        next = MPI_PROC_NULL;
    else
        next = node.rank + 1;

    enum TAGS {
        TO_TOP,
        TO_BOTTOM
    };

    State *const my_start = grid->data + (node.my_y_offset * grid->x);
    State *const temp = (State *) malloc(sizeof(State) * node.my_rows * grid->x);
    int my_summary_offset = node.my_y_offset * grid->x;

    for (int step = 0; step < steps; step++) {
        MPI_Isend(my_start, grid->x, MPI_INT, prev, step * 2 + 1, MPI_COMM_WORLD, req);
        MPI_Irecv(my_start + node.my_rows * grid->x, grid->x, MPI_INT, next, step * 2 + 1, MPI_COMM_WORLD, req + 1);

        MPI_Isend(my_start + (node.my_rows - 1) * grid->x, grid->x, MPI_INT, next, step * 2, MPI_COMM_WORLD, req + 2);
        MPI_Irecv(my_start - grid->x, grid->x, MPI_INT, prev, step * 2, MPI_COMM_WORLD, req + 3);

        MPI_Wait(req + 1, &status);
        MPI_Wait(req + 3, &status);


        //update grid
        for (int y = 0; y < node.my_rows; y++) {
            for (int x = 0; x < grid->x; x++) {
                int heads = 0;
                State *current = temp + index(x, y, grid->x);

                switch (*(grid->data + index(x, y + node.my_y_offset, grid->x))) {
                    case empty:
                        *current = empty;
                        break;
                    case head:
                        *current = tail;
                        break;
                    case tail:
                        *current = conductor;
                        break;
                    case conductor:
                        for (signed int dy = -1; dy < 2; dy++) {
                            for (signed int dx = -1; dx < 2; dx++) {
                                if ((x + dx >= 0) && (x + dx < grid->x) //if not outside x
                                    && (y + node.my_y_offset + dy >= 0) &&
                                    (y + node.my_y_offset + dy < grid->y) //if not outside y
                                    && (*(grid->data + index(x + dx, y + dy + node.my_y_offset, grid->x)) == head)
                                        ) {
                                    heads++;
                                }
                            }
                        }

                        if ((heads == 1) || (heads == 2)) {
                            *current = head;
                        } else {
                            *current = conductor;
                        }
                        break;
                    default:
                        printf("Invalid value %d - int is not in State enum allowed values\n",
                               (int) grid->data[index(x, y, grid->x)]);
                        exit(1);
                        break;
                }
            }
        }
        //save temp
        memcpy(grid->data + my_summary_offset, temp, node.my_rows * grid->x * sizeof(State));
        if (present_simulation)
            present_simulation(grid);
    }
    free(temp);
}