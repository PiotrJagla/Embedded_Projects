#include "rect.h"

void DrawRect(const struct rect* r) {
    DrawRectangle(r->x, r->y, r->width, r->height, r->c);
}