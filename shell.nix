{ debug ? false, pkgs ? import <nixpkgs> {} }:
let callPackage = pkgs.callPackage;
    nanovg      = callPackage ./etc/nix-support/nanovg.nix { };
    genie       = callPackage ./etc/nix-support/genie.nix { };
    nanogui     = callPackage ./etc/nix-support/nanogui.nix { inherit nanovg; };
    bx          = callPackage ./etc/nix-support/bx.nix { };
    bgfx-nanovg = callPackage ./etc/nix-support/bgfx-nanovg.nix {
      inherit bx bgfx nanovg;
    };
    bgfx = callPackage ./etc/nix-support/bgfx.nix {
      inherit bx genie; debugBuild = debug;
    };
in callPackage ./default.nix {
  inherit bgfx bgfx-nanovg bx genie nanovg nanogui;
}
