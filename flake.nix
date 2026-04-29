{
  description = "A Nix-flake-based C/C++ development environment";

  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0"; # stable Nixpkgs

  outputs = { self, ... }@inputs:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        inputs.nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import inputs.nixpkgs { inherit system; };
          }
        );
    in
    {
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.mkShell
          {
            nativeBuildInputs = with pkgs; [
              dotnetCorePackages.sdk_9_0
            ];
            DOTNET_BIN = "${pkgs.dotnetCorePackages.sdk_9_0}/bin/dotnet";

            packages = with pkgs; [
              omnisharp-roslyn
              netcoredbg
            ];
          };
        }
      );
    };
}
