{ bgfx, bx, nanovg, stdenv, fetchFromGitHub }:
stdenv.mkDerivation rec {
  name = "bgfx-nanovg-${version}";
  version = "0.1";

  buildInputs = [
    bgfx
    bx
    nanovg
  ];

  src = ../../deps/bgfx-nanovg;

  makeFlags = [
    "BGFX_PREFIX=${bgfx}"
    "PREFIX=$(out)"
    "SILENT="
  ];

}
