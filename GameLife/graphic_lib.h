#ifndef GRAPHIC_LIB_H
#define GRAPHIC_LIB_H

#include <stdint.h>

int CreateGraphicResources(uint32_t width, uint32_t height);

void DestroyGraphicResources();

int RenderWindowIsOpen();

void ClearFrame();

void DisplayFrame();

void SetPixel(int x, int y, int argb);

#endif  // GRAPHIC_LIB_H
