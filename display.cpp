//
// Created by SG0220005 on 10/25/2015.
//

#include "display.hh"
#include "colors.hh"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winbase.h>
#include <time.h>

using namespace std;
HDC hdc;

void setup_window(Grid *grid);

COLORREF *grid_to_windows_colors(Grid *grid);

#ifdef TEST_DISPLAY
int main(int argc, char *argv[]) {
    Grid *grid;
    if (argc < 2) {
        printf("usage: %s <input_file> [output_file]\n", argv[0]);
        exit(1);
    }
    grid = readPPM(argv[1]);
    printf("Size x=%d, y=%d; sizeof(COLORREF) = %d; SIZEOF(My_COLORREF) = %d\n", grid->x, grid->y, sizeof(COLORREF),
           sizeof(My_COLORREF));
    setup_window(grid);

    display(grid);
    getchar();
}
#endif
void setup_window(Grid *grid) {
    HWND hwnd = CreateWindowA("PROGRESS_CLASS", "dummy", WS_VISIBLE, 10, 10, grid->x, grid->y, NULL, NULL, NULL, NULL);
    hdc = GetDC(hwnd);
}

int display(Grid *grid) {
    COLORREF *arr = grid_to_windows_colors(grid);
    srand(time(NULL));
// Creating temp bitmap
    HBITMAP map = CreateBitmap(grid->x, // width. 512 in my case
                               grid->y, // height
                               1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
                               8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
                               (void *) arr); // pointer to array


    HDC src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd);
    SelectObject(src, map); // Inserting picture into our temp HDC
// Copy image from temp HDC to window
    BitBlt(hdc, // Destination
           0,  // x and
           0,  // y - upper-left corner of place, where we'd like to copy
           grid->x, // width of the region
           grid->y, // height
           src, // source
           0,   // x and
           0,   // y of upper left corner  of part of the source, from where we'd like to copy
           SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;

    DeleteDC(src);
    free(arr);

}
COLORREF *grid_to_windows_colors(Grid *grid) {
    int grid_size = grid->x * grid->y;

    COLORREF *pixels = (COLORREF *) malloc(sizeof(COLORREF) * grid_size);
    if (!pixels) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    for (int i = 0; i < grid_size; i++) {
        switch (grid->data[i]) {
            case empty:
                pixels[i] = EMPTY_PIXEL_COLORREF;
                break;
            case head:
                pixels[i] = HEAD_PIXEL_COLORREF;
                break;
            case tail:
                pixels[i] = TAIL_PIXEL_COLORREF;
                break;
            case conductor:
                pixels[i] = CONDUCTOR_PIXEL_COLORREF;
                break;
            default:
                fprintf(stderr, "Unknown grid state %d\n", (int) grid->data[i]);
                exit(1);
        }
    }
    return pixels;
}