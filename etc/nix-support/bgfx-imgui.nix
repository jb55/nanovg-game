
{ stdenv, fetchFromGitHub, stb}:

stdenv.mkDerivation rec {
  name = "bgfx-imgui-${version}";
  version = "1.47";

  src = ../../deps/ocornut-imgui;

  buildPhase = ''
    gcc -O2 -c -o imgui.o imgui.cpp
    gcc -O2 -c -o imgui_draw.o imgui_draw.cpp
    gcc -O2 -c -o imgui_demo.o imgui_demo.cpp
    gcc -O2 -c -o imgui_wm.o imgui_wm.cpp
    ar rcs libbgfx-imgui.a imgui{_draw,_demo,_wm}.o
  '';

  installPhase = ''
    mkdir -p $out/include/imgui
    mkdir -p $out/lib

    cp libbgfx-imgui.a $out/lib

    ln -s $out/include/imgui $out/include/ocornut-imgui

    cp imconfig.h imgui{_internal,_wm}.h stb{,_rect_pack,_textedit,_truetype}.h \
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
