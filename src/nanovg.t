

require 'c'

terra main(argc : int, argv : &rawstring)
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

	var nvg : &C.NVGcontext = C.nvgCreate(1, 0)
	C.bgfx_set_view_seq(0, true)

  while not C.entry_process_events(&width, &height, &debug, &reset) do
    -- Set view 0 default viewport.
    C.bgfx_set_view_rect(0, 0, 0, width, height)

    -- This dummy draw call is here to make sure that view 0 is cleared
    -- if no other draw calls are submitted to view 0.
    C.bgfx_touch(0)

    -- Use debug font to print information about this example.
    C.bgfx_dbg_text_clear(0, false)
    C.bgfx_dbg_text_printf(0, 1, 0x4f, "nanovg test")
    C.bgfx_dbg_text_printf(0, 2, 0x6f, "Description: NanoVG is small antialiased vector graphics rendering library.")

		C.nvgBeginFrame(nvg, width, height, 1.0)

      C.nvgBeginPath(nvg)
      C.nvgCircle(nvg, 250, 250, 100.0)
      C.nvgFillColor(nvg, C.nvgRGBA(32,32,32,255))
      C.nvgFill(nvg);

		C.nvgEndFrame(nvg);

    -- Advance to next frame. Rendering thread will be kicked to
    -- process submitted rendering primitives.
    C.bgfx_frame()
  end

	C.nvgDelete(nvg);

  C.bgfx_shutdown()

end

terralib.saveobj("nanovg.o", {_main_ = main})
