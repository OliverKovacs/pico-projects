{ pkgs ? import <nixpkgs> {} } :
let
    picoSdkSubmodules = pkgs.pico-sdk.override { withSubmodules = true; };
in
pkgs.mkShell {
    buildInputs = [
        picoSdkSubmodules
        pkgs.cmake
        pkgs.gcc-arm-embedded
        pkgs.git
        pkgs.gnumake
        pkgs.picotool
        pkgs.python3
        pkgs.screen
    ];
    nativeBuildInputs = [];
    shellHook = ''
        export PICO_SDK_PATH=${picoSdkSubmodules}/lib/pico-sdk
    '';
}
