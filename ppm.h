//
// Created by SG0220005 on 10/24/2015.
//

#ifndef WIREWORLD_PPM_H
#define WIREWORLD_PPM_H

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

typedef struct {
    unsigned char red, green, blue;
} PPMPixel;

void writePPM(const char *filename, Grid *grid);

Grid *readPPM(const char *filename);

PPMPixel *grid_to_pixels(Grid *grid);

#endif //WIREWORLD_PPM_H
