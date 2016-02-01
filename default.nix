{ stdenv
, bgfx
, bgfx-nanovg
, bx
, genie
, glibc
, glew
, glfw3
, mesa_glu
, nanovg
, nanogui
, pkgconfig
, premake4
, terra
, gdb
, xlibs
}:

let buildInputs = with xlibs; [
      bgfx
      bgfx-nanovg
      bx
      gdb
      genie
      glew
      glfw3
      libX11
      mesa_glu
      glibc
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

  INCLUDE_PATH=".;./common;${buildPaths ";" (f: "${f}/include")}";

  meta = with stdenv.lib; {
    description = "NanoVG portable game";
    homepage = "https://github.com/jb55/nanovg-game";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
  };
} 
