#include<stdio.h>
#include<stdlib.h>
#include "wireword.h"
#include "ppm.h"
#include "colors.hh"

#define CREATOR "Mateusz Pszczolka"
#define RGB_COMPONENT_COLOR 255

int compare_pixels(const PPMPixel *left, const PPMPixel *right);

int compare_pixels(const PPMPixel *left, unsigned char r, unsigned char g, unsigned char b);

Grid *readPPM(const char *filename) {
    char buff[16];
    Grid *grid;
    FILE *fp;
    int c, rgb_comp_color;
    int grid_size;
    //open PPM file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //read image format
    if (!fgets(buff, sizeof(buff), fp)) {
        perror(filename);
        exit(1);
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }

    //alloc memory form image
    grid = (Grid *) malloc(sizeof(Grid));
    if (!grid) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n');
        c = getc(fp);
    }

    ungetc(c, fp);
    //read image size information
    if (fscanf(fp, "%d %d", &grid->x, &grid->y) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
        exit(1);
    }

    grid_size = grid->x * grid->y;
    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
        exit(1);
    }

    //check rgb component depth
    if (rgb_comp_color != RGB_COMPONENT_COLOR) {
        fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
        exit(1);
    }

    while (fgetc(fp) != '\n');
    //memory allocation for pixel data
    grid->data = (State *) malloc(grid_size * sizeof(State));

    PPMPixel *pixels = (PPMPixel *) malloc(sizeof(PPMPixel) * grid_size);
    //read pixel data from file
    if (!pixels) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    if (fread(pixels, 3 * grid->x, grid->y, fp) != grid->y) {
        fprintf(stderr, "Error loading image '%s'\n", filename);
        exit(1);
    }

    for (int i = 0; i < grid_size; i++) {
        PPMPixel *pixel = &pixels[i];
        if (compare_pixels(pixel, &EMPTY_PIXEL))
            grid->data[i] = empty;
        else if (compare_pixels(pixel, &HEAD_PIXEL))
            grid->data[i] = head;
        else if (compare_pixels(pixel, &TAIL_PIXEL) || compare_pixels(pixel, 0x00, 0xff, 0xff))
            grid->data[i] = tail;
        else if (compare_pixels(pixel, &CONDUCTOR_PIXEL) || compare_pixels(pixel, 0xff, 0x7f, 0x00))
            grid->data[i] = conductor;
        else {
            fprintf(stderr, "Pixel (%d, %d) with %02x%02x%02x' cannot be recogite as grid element'\n",
                    (int) (i % grid->x), (int) (i / grid->x),
                    (int) pixel->red, (int) pixel->green, (int) pixel->blue
            );
        }
    }
    free(pixels);
    fclose(fp);
    return grid;
}

int compare_pixels(const PPMPixel *left, const PPMPixel *right) {
    return (left->green == right->green) && (left->blue == right->blue) && (left->red == right->red);
}

int compare_pixels(const PPMPixel *left, unsigned char r, unsigned char g, unsigned char b) {
    return (left->green == g) && (left->blue == b) && (left->red == r);
}

void writePPM(const char *filename, Grid *grid) {
    FILE *fp;
    int grid_size = grid->x * grid->y;

    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    PPMPixel *pixels = grid_to_pixels(grid);
    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //comments
    fprintf(fp, "# Created by %s\n", CREATOR);

    //image size
    fprintf(fp, "%d %d\n", grid->x, grid->y);

    // rgb component depth
    fprintf(fp, "%d\n", RGB_COMPONENT_COLOR);

    // pixel data
    fwrite(pixels, 3 * grid->x, grid->y, fp);
    free(pixels);
    fclose(fp);
}

PPMPixel *grid_to_pixels(Grid *grid) {
    int grid_size = grid->x * grid->y;

    PPMPixel *pixels = (PPMPixel *) malloc(sizeof(PPMPixel) * grid_size);
    if (!pixels) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    for (int i = 0; i < grid_size; i++) {
        switch (grid->data[i]) {
            case empty:
                pixels[i] = EMPTY_PIXEL;
                break;
            case head:
                pixels[i] = HEAD_PIXEL;
                break;
            case tail:
                pixels[i] = TAIL_PIXEL;
                break;
            case conductor:
                pixels[i] = CONDUCTOR_PIXEL;
                break;
            default:
                fprintf(stderr, "Unknown grid state %d\n", (int) grid->data[i]);
                exit(1);
        }
    }
    return pixels;
}
//
//int main(int argc, char *argv[]) {
//    Grid *grid;
//    if (argc < 2) {
//        printf("usage: %s <input_file> [output_file]\n", argv[0]);
//        exit(1);
//    }
//    grid = readPPM(argv[1]);
//
//    printf("Size x=%d, y=%d", grid->x, grid->y);
//    for (int y = 0; y < grid->y; y++) {
//        printf("\n");
//        for (int x = 0; x < grid->x; x++) {
//            printf("%d ", grid->data[x + y * grid->x]);
//        }
//    }
//    if (argc >= 3)
//        writePPM(argv[2], grid);
//}