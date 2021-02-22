{ stdenv
, lib
, pkgs
, cmake
, ninja
, mesa
, mesa_drivers
, doxygen
, glfw3
}:

let
  stdenvClang              = pkgs.clangStdenv;
in
  stdenvClang.mkDerivation {
    name                   = "automated-tending";
    version                = "1.0";
    src                    = builtins.filterSource (p: t: lib.cleanSourceFilter p t && baseNameOf p != "build") ./.;

    nativeBuildInputs      = [
      cmake
      ninja
    ];

    propagatedBuildInputs  = [
      mesa
      mesa_drivers
    ];
  
    buildInputs            = [
      glfw3
      doxygen
    ];

    cmakeFlags             = [
      "-DCMAKE_BUILD_TYPE=Release"
      "-GNinja"
    ];

    preConfigure           = ''
      cmakeFlags="$cmakeFlags"
    '';

    preBuild               = ''
      ninja clean
    '';

    buildPhase             = ''
      ninja
    '';

    makeTarget             = "automated-tending";
    enableParallelBuilding = true;

    doCheck                = false;
    checkTarget            = "test";

    installPhase           = ''
      mkdir -p $out/bin $out/bin/logs $out/bin/external $out/nix-support;
      cp -r conf external main $out/bin/;
    '';
  }
