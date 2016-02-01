{ stdenv
, fetchFromGitHub
, glfw3
, glew
, eigen
, cmake
, nanovg
, mesa_glu
, xlibs
}:

stdenv.mkDerivation rec {
  name = "nanogui-git-${version}";
  version = "2015-10-25";

  src = fetchFromGitHub {
    owner = "jb55";
    repo = "nanogui";
    rev = "f457366f3f8d4a48ac9631c0c086ea561ac57b6c";
    sha256 = "10272cnizvq3lx1s62d4lk4cx41blrwkm3vz8bjq9iplnml6f15q";
  };

  cmakeFlags = [
    "-DNANOGUI_BUILD_SHARED='OFF'"
    "-DNANOGUI_BUILD_PYTHON='OFF'"
    "-DNANOGUI_BUILD_PYTHON='OFF'"
    "-DNANOGUI_HAS_PARENT='ON'"
  ];

  installPhase = ''
    mkdir -p $out/{lib,include}
    cp libnanogui.a $out/lib
    cp -r ../include $out
  '';

  buildInputs = [ cmake glfw3 eigen glew nanovg ] ++
    (with xlibs; [ libXrandr libXinerama libXcursor libXi libX11 ]);

  meta = with stdenv.lib; {
    description = "nanovg";
    homepage = "https://github.com/memononen/nanovg";
    maintainers = with maintainers; [ jb55 ];
    license = licenses.bsd3;
  };
}
