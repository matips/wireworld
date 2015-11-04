//
// Created by SG0220005 on 10/25/2015.
//

#ifndef WIREWORLD_COLORS_H
#define WIREWORLD_COLORS_H

#include <windows.h>
#include "ppm.h"

const PPMPixel EMPTY_PIXEL = {0, 0, 0};
const PPMPixel HEAD_PIXEL = {0xff, 0xff, 0xff};
const PPMPixel TAIL_PIXEL = {0x00, 0x80, 0xff};
const PPMPixel CONDUCTOR_PIXEL = {0xff, 0x80, 0x00};

typedef union {
    struct {
        unsigned char blue;
        unsigned char green;
        unsigned char read;
        unsigned char zero;
    } PPM;
    COLORREF color;
} My_COLORREF;

static COLORREF PPMPixel_to_COLORREF(PPMPixel ppmpixel) {
    My_COLORREF my_color;
    my_color.PPM.zero = 0;
    my_color.PPM.blue = ppmpixel.blue;
    my_color.PPM.green = ppmpixel.green;
    my_color.PPM.read = ppmpixel.red;
    return my_color.color;
}

const COLORREF EMPTY_PIXEL_COLORREF = PPMPixel_to_COLORREF(EMPTY_PIXEL);
const COLORREF HEAD_PIXEL_COLORREF = PPMPixel_to_COLORREF(HEAD_PIXEL);
const COLORREF TAIL_PIXEL_COLORREF = PPMPixel_to_COLORREF(TAIL_PIXEL);
const COLORREF CONDUCTOR_PIXEL_COLORREF = PPMPixel_to_COLORREF(CONDUCTOR_PIXEL);

#endif //WIREWORLD_COLORS_H
