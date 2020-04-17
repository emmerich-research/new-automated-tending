{
  pkgs    ? import <nixpkgs> {}
}:

pkgs.callPackage (import ./derivation.nix) {}
