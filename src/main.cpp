#include "grid/grid.hpp"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define BOX_DIMENSION 10

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Game of life");

    SetTargetFPS(30);

    const uint box_width = WIDTH / BOX_DIMENSION;
    const uint box_height = HEIGHT / BOX_DIMENSION;

    grid::abstract_grid *grid =
        new grid::set_grid(box_width, box_height, 0.5);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        for (int i = 0; i < box_width; ++i) {
            for (int j = 0; j < box_height; ++j) {
                DrawRectangle(BOX_DIMENSION * i, BOX_DIMENSION * j,
                              BOX_DIMENSION - 1, BOX_DIMENSION - 1,
                              grid::CellState::alive == grid->get_elem(i, j)
                                  ? WHITE
                                  : BLACK);
            }
        }

        DrawFPS(10, 10);
        grid->run_gol_step();
        EndDrawing();
    }

    delete grid;

    CloseWindow();
    return 0;
}
