

local C = terralib.includecstring [[
  #include <bgfx/c99/bgfx.h>
  #include <stdio.h>
  #include "logo.h"
  #include <nanovg/nanovg.h>
  #include <math.h>
  #include <hp-counter/hp-counter.h>
  #include <bgfx-nanovg/bgfx-nanovg.h>

  extern bool entry_process_events(uint32_t* _width, uint32_t* _height, uint32_t* _debug, uint32_t* _reset);
]]

function tuint32 (v)
  return constant(uint32, v)
end

BGFX_DEBUG_TEXT = tuint32(0x00000008)
BGFX_RESET_VSYNC = tuint32(0x00000080)
BGFX_CLEAR_COLOR = tuint32(0x0001)
BGFX_CLEAR_DEPTH = tuint32(0x0002)

return C
