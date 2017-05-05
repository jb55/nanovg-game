

C = require 'c'

function rgb(r,g,b,a)
  return `C.nvgRGBA(r,g,b,a)
end

function testlang(nvg, t)
  import "lang/nvg"
  ctx nvg
  beginpath
  local x = `C.cosf(t * 2f) * 100f
  circle x 100 200
  fillcolor rgba 255 255 255 255
  fill
  return done
end

white = `C.nvgRGBA(255.0, 255.0, 255.0, 255.0)

function target(nvg, x, y, r)
  import "lang/nvg"
  ctx nvg
  beginpath
  circle x y r
  fillcolor (rgba 255 0 0 255)
  fill
  local r2 = `r * 0.75f
  beginpath
  circle x y r2
  fillcolor (rgba 255 255 255 255)
  fill
  local r3 = `r * 0.5f
  beginpath
  circle x y r3
  fillcolor (rgba 255 0 0 255)
  fill
  local r4 = `r * 0.25f
  beginpath
  circle x y r4
  fillcolor white
  fill
  return done
end

terra target_game(nvg : &C.NVGcontext, game : &Game, t : float)
  var c = 2.0f
  var r = C.abs(C.cosf(t) * 100f + 50f);
  var x = C.abs(C.cosf(t) * game.width / c + game.width / (c*2.0))
  var y = C.abs(C.sinf(t) * game.height / c + game.height / (c*2.0))
  [ target(nvg, x, y, r) ]
end

-- 500 x 500
-- 15 pix spacing
-- 105 x 105 blocks

-- highlighted tile: 238 228 218
-- back tile: 205 193 180
-- 4 tile: 237 224 200

struct Game {
  vg : &C.NVGcontext;
  bx : float;
  by : float;
  width : float;
  height : float;
  font_normal : int;
}

terra load_data(game : &Game) : int
  var vg = game.vg;
	game.font_normal = C.nvgCreateFont(vg, "sans", "runtime/font/roboto-regular.ttf");
  if game.font_normal == -1 then
    C.printf("Could not load regular font\n")
    return -1
  end
  C.printf("Loaded font with index: %d\n", game.font_normal)
  return 1
end

terra draw_tile(game : &Game, x : float, y : float, ts : int, n : int)
  var vg = game.vg;
  var bx = game.bx;
  var by = game.by;

  var ths = ts / 2;
  var tx = bx + ((15 * (x+1)) + (ts * x))
  var ty = by + ((15 * (y+1)) + (ts * y))

  C.nvgBeginPath(vg);

  C.nvgRoundedRect(vg, tx, ty, ts, ts, 10);
  C.nvgFillColor(vg, [rgb(238, 228, 218, 255)]);
  C.nvgFill(vg);

	--C.nvgFontSize(vg, 24.0f);
	--C.nvgFontFaceId(vg, game.font_normal);
	--C.nvgFontFace(vg, "sans");
  --C.nvgFillColor(vg, C.nvgRGBA(0,0,0,255));
	--C.nvgTextAlign(vg, C.NVG_ALIGN_LEFT or C.NVG_ALIGN_MIDDLE);
  --C.nvgText(vg, 10, 10, "a", nil);

end

terra draw_tiles(game : &Game)
  var ts = 105

  -- TODO: centered
  for x = 0,4 do
    for y = 0,4 do
      draw_tile(game, x, y, ts, 0)
    end
  end
end

terra draw_board(game : &Game)
  var size = 500
  var bx = game.bx
  var by = game.by
  var vg = game.vg

  var bg = C.nvgLinearGradient(vg, bx, by, bx+size, by+size,
                               C.nvgRGBA(187,173,160,255),
                               C.nvgRGBA(159,147,136,255));

	C.nvgBeginPath(vg);
  -- TODO: centered
	C.nvgRoundedRect(vg, bx, by, size, size, 20);
	C.nvgFillPaint(vg, bg);
	C.nvgFill(vg);

  draw_tiles(game)
end


terra render_demo(game : &Game, width : float, height : float, t : float)
  var vg = game.vg
  draw_board(game)
  --[ testlang(vg, t) ]
  target_game(vg, game, t)
end

terra errors(err : rawstring)
  C.printf("%s\n", err)
  C.exit(1)
end

terra main(argc : int, argv : &rawstring)
  var width : uint32 = 1280
  var height : uint32 = 720
  var reset  = BGFX_RESET_VSYNC
  var debug  = BGFX_DEBUG_TEXT

  var game : Game
  game.bx = 200
  game.by = 200

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
  game.vg = nvg

  var time_offset = C.hp_counter()
  var last : int64

  if load_data(&game) == -1 then
    errors("Failed to load data, make sure the cwd is right")
  end

  while not C.entry_process_events(&width, &height, &debug, &reset) do
    var now = C.hp_counter()
		var frameTime : int64 = now - last
    last = now
    var freq : double = C.hp_frequency()
    var time : float = (now - time_offset) / freq
    var ms : double = 1000.0 / freq

    -- Set view 0 default viewport.
    C.bgfx_set_view_rect(0, 0, 0, width, height)
    game.width = width;
    game.height = height;

    -- This dummy draw call is here to make sure that view 0 is cleared
    -- if no other draw calls are submitted to view 0.
    C.bgfx_touch(0)

    -- Use debug font to print information about this example.
    C.bgfx_dbg_text_clear(0, false)
    C.bgfx_dbg_text_printf(0, 1, 0x4f, "nanovg test")
    C.bgfx_dbg_text_printf(0, 2, 0x6f, "Description: NanoVG is small antialiased vector graphics rendering library.")
    C.bgfx_dbg_text_printf(0, 3, 0x2f, "frame time: %f", frameTime)

		C.nvgBeginFrame(nvg, width, height, 1.0)

    render_demo(&game, width, height, time)

		C.nvgEndFrame(nvg);

    -- Advance to next frame. Rendering thread will be kicked to
    -- process submitted rendering primitives.
    C.bgfx_frame()
  end

	C.nvgDelete(nvg);

  C.bgfx_shutdown()

end

terralib.saveobj("nanovg.to", "object", {_main_ = main})
terralib.saveobj("nanovg.ll", {_main_ = main})
terralib.saveobj("nanovg.s", {_main_ = main})
