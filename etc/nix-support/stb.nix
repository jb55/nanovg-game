
{ stdenv, fetchFromGitHub }:

stdenv.mkDerivation rec {
  name = "stb-${version}";
  version = "955dfe991b26f6fb1287ec0093f606843487b099";

  src = fetchFromGitHub {
    owner = "nothings";
    repo = "stb";
    rev = version;
    sha256 = "0w0r8d2qbcdrx6347irgzkq7p40qhc8wjaa7sawmv2ibq35v3pad";
  };

  installPhase = ''
    mkdir -p $out/include
    cp -r * $out/include
  '';

  meta = with stdenv.lib; {
    description = "Generic header util lib";
    homepage = "https://github.com/nothings/stb";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
    platforms = platforms.all;
  };
}
