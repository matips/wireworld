#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <unistd.h>
#include <time.h>
#include "wireword.h"
#include "ppm.h"
#include "measure.h"
#define BILLION 1E9


int main(int argc, char *argv[]) {
    //display step if needed
    Grid grid;


    int steps;

    if (argc < 4) {
        printf("usage: %s <width> <height> <steps>\n", argv[0]);
        exit(1);
    }
    MPI_Init(NULL, NULL);
    grid.y = atoi(argv[1]);
    grid.x = atoi(argv[2]);
    steps = atoi(argv[3]);
    grid.data = (State *) malloc(grid.x * grid.y * sizeof(State));

    if (!grid.data) {
        printf("Cannot alocate memory for grid");
        exit(1);
    }
    Node_config node = get_node(&grid);

    srand(time(NULL));
    for (int x = 0; x < grid.x; x++) {
        for (int y = 0; y < node.my_rows; y++) {
            *(grid.data + x + (y + node.my_y_offset) * grid.x) = (State) (rand() % 4);
        }
    }


    double start_time = MPI_Wtime();

    wireworld(&grid, steps, NULL);
    MPI_Barrier(MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (!node.rank) {
        printf("%d; %d; %d; %d; %f\n", node.world_size, grid.y, grid.x, steps, (end_time - start_time));
    }
    MPI_Finalize();
}
