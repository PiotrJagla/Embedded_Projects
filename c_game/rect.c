#include "rect.h"

void DrawRect(const struct rect* r, Color c) {
    DrawRectangle(r->x, r->y, r->width, r->height, c);
}