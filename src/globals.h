
#ifndef GLOBALS_H
#define GLOBALS_H

#include <nanovg/nanovg.h>

#define v2cpv(v2) (cpv((v2).x, (v2).y))
#define cpv2(cp) (vec2((cp).x, (cp).y))

extern struct NVGcontext *nvg;
extern float g_stroke;

#endif /* GLOBALS_H */
