#include <stdlib.h>

#include "cell_field.h"
#include "graphic_lib.h"

void SetPixels(CellField *field)
{
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            SetPixel(j, i, field->cells[i][j]);
        }
    }
}

int main()
{
    if (!CreateGraphicResources(WIDTH, HEIGHT)) {
        return EXIT_FAILURE;
    }

    CellField field;
    CellField_Initialize(&field);

    while (RenderWindowIsOpen()) {
        ClearFrame();
        CellField_Update(&field);
        SetPixels(&field);
        DisplayFrame();
    }

    DestroyGraphicResources();
    return EXIT_SUCCESS;
}
