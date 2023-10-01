#ifndef CELL_FIELD_H
#define CELL_FIELD_H

typedef int cell_t;

typedef struct {

#define HEIGHT 720
#define WIDTH 1280
#define FERTILITY_FACTOR 5

    cell_t cells[HEIGHT][WIDTH];
} CellField;

void CellField_Initialize(CellField *field);

void CellField_Update(CellField *field);

void CellField_Copy(CellField *src_field, CellField *dst_field);

void CellField_MarkCell(CellField *field, CellField *next_field, int i, int j);

void CellField_SetNearbyCells(CellField *field, int *nearby_cells, int i, int j);

int CellField_IsCellDead(cell_t cell);

#endif  // CELL_FIELD_H
