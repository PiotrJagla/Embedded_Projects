#include "colors.h"

bool CompareColors(Color a, Color b) {
  return a.r == b.r && a.g == b.g && a.b == b.b;
}
bool PrintColor(Color a){
    printf("R: %d, G: %d, B: %d, A: %d\n", a.r, a.g, a.b, a.a);
}