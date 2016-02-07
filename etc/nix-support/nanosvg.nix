{ stdenv, fetchFromGitHub }:

stdenv.mkDerivation rec {
  name = "nanosvg-${version}";
  version = "a523e0215f3f99d44b2dcfdf241e0e27a75e35f0";

  src = fetchFromGitHub {
    owner = "memononen";
    repo = "nanosvg";
    rev = version;
    sha256 = "02dg227hdbyqy2a0ci0nw1mw2arisbdapgchx7a4pn5h1rkqcgjr";
  };

  installPhase = ''
    mkdir -p $out/include/nanosvg
    cp src/nanosvg{,rast}.h $out/include/nanosvg
  '';

  meta = with stdenv.lib; {
    description = "nanosvg";
    homepage = "https://github.com/memononen/nanosvg";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.zlib;
  };
}
