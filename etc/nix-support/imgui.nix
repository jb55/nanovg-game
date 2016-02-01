{ stdenv, fetchFromGitHub, imconfig ? "" }:

stdenv.mkDerivation rec {
  name = "imgui-${version}";
  version = "1.47";

  src = fetchFromGitHub {
    owner = "ocornut";
    repo = "imgui";
    rev = "24d186753eea8ebd9a1a10e4bdad5f345e891c9c";
    sha256 = "0w0r8d2qbcdrx6347irgzkq7p40qhc8wjaa7sawmv2ibq35v3pak";
  };

  buildPhase = ''
    gcc -O2 -c -o imgui.o imgui.cpp
    gcc -O2 -c -o imgui_draw.o imgui_draw.cpp
    gcc -O2 -c -o imgui_demo.o imgui_demo.cpp
    ar rcs libimgui.a imgui{_draw,_demo}.o
  '';

  installPhase = ''
    mkdir -p $out/include/imgui
    mkdir -p $out/lib

    cp libimgui.a $out/lib

    ln -s $out/include/imgui $out/include/ocornut-imgui

    cp imconfig.h imgui.h imgui_internal.h stb_rect_pack.h stb_textedit.h stb_truetype.h \
      $out/include/imgui
  '';

  meta = with stdenv.lib; {
    description = "Immediate mode GUI library";
    longDescription = ''
      Bloat-free Immediate Mode Graphical User interface for C++ with minimal
      dependencies
    '';
    homepage = "https://github.com/ocornut/imgui";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.mit;
    platforms = platforms.all;
  };
}
