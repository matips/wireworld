#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>
#include "simulation.h"
#include "display.hh"
#include "wireword.h"
#include "ppm.h"

int pause_step;

void present_step(Grid *grid) {
    MPI_Status status;
    Node_config node = get_node(grid);
    State *const my_start = grid->data + (node.my_y_offset * grid->x);

    if (node.rank == 0) {
        MPI_Request *sim_req = (MPI_Request *) malloc(sizeof(MPI_Request) * node.world_size);
        for (int i = 0; i < node.world_size; i++) {
            if (i != node.rank) {
                int his_rows = grid->y / node.world_size + ((i < (grid->y % node.world_size)) ? 1 : 0);
                int his_y_offset = i * (grid->y / node.world_size);
                if (i > (grid->y % node.world_size)) his_y_offset += (grid->y % node.world_size);
                else his_y_offset += i;

                MPI_Irecv(grid->data + his_y_offset * grid->x, his_rows * grid->x, MPI_INT, i, 0,
                          MPI_COMM_WORLD, sim_req + i);
            }
        }
        for (int i = 0; i < node.world_size; i++) {
            if (i != node.rank) {
                MPI_Wait(sim_req + i, &status);
            }
        }
        free(sim_req);
        display(grid);
        if (pause_step)
            usleep(pause_step * 1000);
    } else {
        MPI_Request send_req;
        MPI_Isend(my_start, grid->x * node.my_rows, MPI_INT, 0, 0, MPI_COMM_WORLD, &send_req);
        MPI_Wait(&send_req, &status);
    }
}


int main(int argc, char *argv[]) {
    int rank, world_size;
    //display step if needed
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int steps;
    char *file_name;
    Grid *grid;

    if (argc < 3) {
        printf("usage: %s <file_name> <steps> <pause>\n", argv[0]);
        exit(1);
    }
    file_name = argv[1];
    steps = atoi(argv[2]);
    if (argc >= 4)
        pause_step = atoi(argv[3]);

    grid = readPPM(file_name);
    if (!rank) {
        setup_window(grid);
        display(grid);
    }

    wireworld(grid, steps, present_step);

}
