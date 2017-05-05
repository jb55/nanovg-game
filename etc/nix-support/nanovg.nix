{ stdenv
, fetchFromGitHub
, glfw3
, premake4
, pkgconfig
, mesa_glu
, glew
, debug
}:

let
  config = if debug then "debug" else "release";
in
stdenv.mkDerivation rec {
  name = "nanovg-git-${version}";
  version = "2015-10-25";

  src = fetchFromGitHub {
    owner = "memononen";
    repo = "nanovg";
    rev = "cb6fd9100d1f316fc14a5b49df40d3defefd799d";
    sha256 = "0bvqln11pxnh9ip9r10ncv4l8lbaz9prdywvhhrhaq3yrjm7r5r4";
  };

  configurePhase = ''
    premake4 gmake
    cd build
  '';

  makeFlags = [ "PREFIX=$(out)" "config=${config}" ];

  installPhase = ''
    mkdir -p $out/{lib,include/nanovg}
    cp libnanovg.a $out/lib
    cp "../src/"* $out/include
    cp "../src/"* $out/include/nanovg
  '';

  buildInputs = [ mesa_glu pkgconfig premake4 glfw3 glew ];

  dontStrip = debug;

  meta = with stdenv.lib; {
    description = "nanovg";
    homepage = "https://github.com/memononen/nanovg";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.zlib;
  };
}
