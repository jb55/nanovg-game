
require 'c'

terra max(a : uint16, b : uint16) : uint16
	if a < b then return b else return a end
end

function rep(expr, n)
  return quote
    for i = 0, n do
      expr
    end
  end
end

terra main(argc : int, argv : &rawstring)
  [rep(`C.printf("hello, world\n"), 10)]

  var width : uint32 = 1280
  var height : uint32 = 720
  var reset  = BGFX_RESET_VSYNC
  var debug  = BGFX_DEBUG_TEXT

  C.bgfx_init(C.BGFX_RENDERER_TYPE_COUNT, 0, 0, nil, nil)
  C.bgfx_reset(width, height, reset)
  C.bgfx_set_debug(debug)

  C.bgfx_set_view_clear(0
    , BGFX_CLEAR_COLOR or BGFX_CLEAR_DEPTH
    , 0x303030ff
    , 1.0f
    , 0
    )

  while not C.entry_process_events(&width, &height, &debug, &reset) do
    -- Set view 0 default viewport.
    C.bgfx_set_view_rect(0, 0, 0, width, height)

    -- This dummy draw call is here to make sure that view 0 is cleared
    -- if no other draw calls are submitted to view 0.
    C.bgfx_touch(0)

    -- Use debug font to print information about this example.
    C.bgfx_dbg_text_clear(0, false)
    C.bgfx_dbg_text_image(max(width/2/8, 20)-20
             , max(height/2/16, 6)-6
             , 40
             , 12
             , &C.s_logo
             , 160
             )
    C.bgfx_dbg_text_printf(0, 1, 0x4f, "bgfx/examples/25-c99")
    C.bgfx_dbg_text_printf(0, 2, 0x6f, "Description: Initialization and debug text with C99 API.")

    -- Advance to next frame. Rendering thread will be kicked to
    -- process submitted rendering primitives.
    C.bgfx_frame()
  end

  C.bgfx_shutdown()

end

terralib.saveobj("hello.o", {_main_ = main})
