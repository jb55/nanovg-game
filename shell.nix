{ debug ? false, pkgs ? import <nixpkgs> {} }:
let callPackage = pkgs.callPackage;
    bgfx-imgui  = callPackage ./etc/nix-support/bgfx-imgui.nix { inherit stb; };
    bx          = callPackage ./etc/nix-support/bx.nix { };
    genie       = callPackage ./etc/nix-support/genie.nix { };
    nanogui     = callPackage ./etc/nix-support/nanogui.nix { inherit nanovg; };
    nanovg      = callPackage ./etc/nix-support/nanovg.nix { };
    stb         = callPackage ./etc/nix-support/stb.nix { };
    bgfx-nanovg = callPackage ./etc/nix-support/bgfx-nanovg.nix {
      inherit bx bgfx nanovg;
    };
    bgfx = callPackage ./etc/nix-support/bgfx.nix {
      inherit bx genie; debugBuild = debug;
    };
in callPackage ./default.nix {
  inherit bgfx bgfx-nanovg bx genie nanovg nanogui bgfx-imgui;
}
