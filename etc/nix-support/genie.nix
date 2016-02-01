{ stdenv, fetchFromGitHub }:
stdenv.mkDerivation rec {
  name = "genie-git-${version}";
  version = "0.1";

  src = fetchFromGitHub {
    owner = "bkaradzic";
    repo = "genie";
    rev = "3ca5982b932d75357ca1ec46bfc145048c931149";
    sha256 = "0w1c33s6gg909lslv32nyrl7bckpl53b6mhpjn3lzv9haw1j99bp";
  };

    installPhase = ''
    mkdir -p $out/bin
    cp bin/linux/genie $out/bin
  '';
}
