{
  description = "C++ Development Environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      llvm = pkgs.llvmPackages_21;
    in
    {
      devShells.${system}.default =
        pkgs.mkShell.override
          {
            stdenv = llvm.stdenv;
          }
          {
            nativeBuildInputs = with pkgs; [
              llvm.clang-tools
              llvm.lldb
              cmake
              ninja
              mold-wrapped
              neocmakelsp
            ];

            buildInputs = with pkgs; [
              llvm.libcxx
              libGL
              glfw
              glew
              gtest
            ];
          };
    };
}
