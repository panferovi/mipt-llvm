#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

typedef struct {
    sfRenderWindow *window;
    sfEvent event;
    sfRectangleShape *pixel;
} GraphicResources;

static GraphicResources graphic_resources;

int CreateGraphicResources(uint32_t width, uint32_t height)
{
    sfVideoMode mode = {width, height, 32};
    graphic_resources.window = sfRenderWindow_create(mode, "Game Life", sfClose, NULL);
    if (!graphic_resources.window) {
        return 0;
    }
    graphic_resources.pixel = sfRectangleShape_create();
    sfVector2f pixel_size = {1, 1};
    sfRectangleShape_setSize(graphic_resources.pixel, pixel_size);
    return 1;
}

void DestroyGraphicResources()
{
    sfRenderWindow_destroy(graphic_resources.window);
    sfRectangleShape_destroy(graphic_resources.pixel);
}

int RenderWindowIsOpen()
{
    return sfRenderWindow_isOpen(graphic_resources.window);
}

void ClearFrame()
{
    while (sfRenderWindow_pollEvent(graphic_resources.window, &graphic_resources.event)) {
        if (graphic_resources.event.type == sfEvtClosed) {
            sfRenderWindow_close(graphic_resources.window);
        }
    }
    sfRenderWindow_clear(graphic_resources.window, sfBlack);
}

void DisplayFrame()
{
    sfRenderWindow_display(graphic_resources.window);
}

void SetPixel(int x, int y, int argb)
{
    if (argb == 0) {
        return;
    }

    sfRectangleShape_setFillColor(graphic_resources.pixel, sfGreen);
    sfVector2f pixel_position = {x, y};
    sfRectangleShape_setPosition(graphic_resources.pixel, pixel_position);
    sfRenderWindow_drawRectangleShape(graphic_resources.window, graphic_resources.pixel, NULL);
}

int GetRandomInteger()
{
    static int is_initialized = 0;
    if (!is_initialized) {
        srand(time(NULL));
        is_initialized = 1;
    }
    return rand();
}
