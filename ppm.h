//
// Created by SG0220005 on 10/24/2015.
//

#ifndef WIREWORLD_PPM_H
#define WIREWORLD_PPM_H

#include "wireword.h"

typedef struct {
    unsigned char red, green, blue;
} PPMPixel;

void writePPM(const char *filename, Grid *grid);

Grid *readPPM(const char *filename);

PPMPixel *grid_to_pixels(Grid *grid);

#endif //WIREWORLD_PPM_H
