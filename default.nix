{ stdenv
, bgfx
, bgfx-nanovg
, bgfx-imgui
, bx
, gdb
, genie
, glew
, glfw3
, glibc
, mesa_glu
, nanogui
, nanovg
, pkgconfig
, premake4
, terra
, xlibs
}:

let buildInputs = with xlibs; [
      bgfx
      bgfx-nanovg
      bgfx-imgui
      bx
      gdb
      genie
      glew
      glfw3
      glibc
      libX11
      mesa_glu
      nanogui
      nanovg
      pkgconfig
      premake4
      terra
    ];
    id = v: v;
    buildPaths = sep: fmt:
      "${stdenv.lib.concatStringsSep sep (map fmt buildInputs)}";
in stdenv.mkDerivation rec {
  inherit buildInputs;

  name = "nanovg-game";
  version = "0.1";

  src = ./.;

  makeFlags = "PREFIX=$(out)";

  NIX_CFLAGS_COMPILE = "-I${bgfx}/share/bgfx/common";
  INCLUDE_PATH=".;${bgfx}/share/bgfx/common;${buildPaths ";" (f: "${f}/include")}";

  meta = with stdenv.lib; {
    description = "NanoVG game";
    homepage = "https://github.com/jb55/nanovg-game";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
  };
}
