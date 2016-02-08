
#include <stdio.h>
#include <string.h>
#include <math.h>
#define NANOSVG_IMPLEMENTATION
#include <nanosvg/nanosvg.h>
#include <nanovg/nanovg.h>
#include <bgfx/bgfx.h>
#include <stddef.h>

NVGcolor nvgColorU32(uint32_t color) {
  return nvgRGB(color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
}

void nvgDrawSVG(struct NVGcontext *vg, struct NSVGimage *image) {
  for (auto shape = image->shapes; shape != NULL; shape = shape->next) {
    nvgStrokeWidth(vg, shape->strokeWidth);
    uint32_t color = shape->stroke.color;
    NVGcolor ncolor = nvgColorU32(color);
    nvgStrokeColor(vg, nvgRGB(255, 255, 255));
    // nvgStrokeColor(vg, ncolor);

    nvgBeginPath(vg);
    for (auto path = shape->paths; path != NULL; path = path->next) {
      nvgMoveTo(vg, path->pts[0], path->pts[1]);
      for (int i = 0; i < path->npts-1; i += 3) {
        float* p = &path->pts[i*2];
        nvgBezierTo(vg, p[2], p[3], p[4], p[5], p[6], p[7]);
      }
      if (path->closed) {
        nvgFillColor(vg, ncolor);
        nvgFill(vg);
        nvgLineTo(vg, path->pts[0], path->pts[1]);
      }
      nvgStroke(vg);
    }
  }
}


void nanosvgTest(struct NVGcontext *vg, struct NSVGimage *image) {
  nvgMoveTo(vg, 500, 500);
  nvgDrawSVG(vg, image);

  // nvgBeginPath(vg);
  // nvgStrokeWidth(vg, 100);
  // nvgStrokeColor(vg, nvgRGB(255, 255, 255));
  // nvgLineTo(vg, 500, 500);
  // nvgStroke(vg);

}
