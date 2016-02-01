#pragma once

// bgfx-nanovg functions
NVGcontext* nvgCreate(int edgeaa, unsigned char viewid);
void nvgViewId(struct NVGcontext*x, unsigned char viewid);
void nvgDelete(struct NVGcontext*x);
