
#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_IMPLEMENTATION
#include <nanosvg/nanosvg.h>
#include <nanovg/nanovg.h>
#include <stddef.h>

void nvgDrawSVG(struct NVGcontext *vg, struct NSVGimage *image) {
  for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
    nvgStrokeWidth(vg, shape->strokeWidth);
    int color = shape->stroke.color;
    nvgStrokeColor(vg, nvgRGB(color & 0xFF,
                              (color >> 8) & 0xFF,
                              (color >> 16) & 0xFF));
    for (auto path = shape->paths; path != NULL; path = path->next) {
      nvgBeginPath(vg);
      nvgMoveTo(vg, path->pts[0], path->pts[1]);
      for (int i = 0; i < path->npts-1; i += 3) {
        float* p = &path->pts[i*2];
        nvgBeginPath(vg);
        nvgBezierTo(vg, p[2], p[3], p[4], p[5], p[6], p[7]);
      }
      if (path->closed)
        nvgLineTo(vg, path->pts[0], path->pts[1]);
      nvgStroke(vg);
    }
  }
}


void nanosvgTest(struct NVGcontext *vg, struct NSVGimage *image) {
  nvgDrawSVG(vg, image);
}
