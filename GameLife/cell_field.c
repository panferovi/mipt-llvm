#include "cell_field.h"

extern int GetRandomInteger();

cell_t CellField_dead_cell = 0;
cell_t CellField_live_cell = 1;

void CellField_Initialize(CellField *field)
{
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            field->cells[i][j] = !(GetRandomInteger() % FERTILITY_FACTOR);
        }
    }
}

void CellField_Update(CellField *field)
{
    CellField next_field;
    CellField_Copy(field, &next_field);

    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            CellField_MarkCell(field, &next_field, i, j);
        }
    }
    CellField_Copy(&next_field, field);
}

void CellField_Copy(CellField *src_field, CellField *dst_field)
{
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            dst_field->cells[i][j] = src_field->cells[i][j];
        }
    }
}

void CellField_MarkCell(CellField *field, CellField *next_field, int i, int j)
{
    static const int nearby_cells_cnt = 8;
    int nearby_cells[8] = {};
    CellField_SetNearbyCells(field, nearby_cells, i, j);

    int live_cells = 0;
    for (int i = 0; i < nearby_cells_cnt; ++i) {
        if (!CellField_IsCellDead(nearby_cells[i])) {
            live_cells++;
        }
    }

    if (CellField_IsCellDead(field->cells[i][j])) {
        if (live_cells == 3) {
            next_field->cells[i][j] = CellField_live_cell;
        }
    } else {
        if (live_cells != 2 && live_cells != 3) {        
            next_field->cells[i][j] = CellField_dead_cell;
        }
    }
}

void CellField_SetNearbyCells(CellField *field, int *nearby_cells, int i, int j)
{
    int up_idx = i != 0 ? i - 1 : HEIGHT - 1;
    int down_idx = i != HEIGHT - 1 ? i + 1 : 0;    
    int left_idx = j != 0 ? j - 1 : WIDTH - 1;
    int right_idx = j != WIDTH - 1 ? j + 1 : 0;

    nearby_cells[0] = field->cells[up_idx][left_idx];
    nearby_cells[1] = field->cells[up_idx][j];
    nearby_cells[2] = field->cells[up_idx][right_idx];
    nearby_cells[3] = field->cells[i][left_idx];
    nearby_cells[4] = field->cells[i][right_idx];
    nearby_cells[5] = field->cells[down_idx][left_idx];
    nearby_cells[6] = field->cells[down_idx][j];
    nearby_cells[7] = field->cells[down_idx][right_idx];
}

int CellField_IsCellDead(cell_t cell) {
    return cell == CellField_dead_cell;
}
