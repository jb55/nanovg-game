{ fetchFromGitHub, stdenv }:
stdenv.mkDerivation rec {
  name = "bx-git-${version}";
  version = "0.1";

  src = fetchFromGitHub {
    owner = "bkaradzic";
    repo = "bx";
    rev = "c7b73760c704270eb2312738797c026bd69237bb";
    sha256 = "0wydg14frp6flgqvk67c47609ymcdm33lfk9dcc4a65z5sidlk4k";
  };

  phases = ["unpackPhase" "installPhase"];

  installPhase = ''
    mkdir -p $out
    cp -r * $out
  '';
}
