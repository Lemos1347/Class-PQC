{
  description = "Development environment with liboqs and advapi32";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      # Supported systems
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      # Helper function to generate attributes for all systems
      forAllSystems =
        function:
        nixpkgs.lib.genAttrs supportedSystems (
          system:
          function {
            pkgs = nixpkgs.legacyPackages.${system};
            inherit system;
          }
        );

      # Shell definition
      makeShells =
        { pkgs, system }:
        {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              liboqs
            ];

            shellHook = ''
              echo "🔐 Quantum-Safe Cryptography Development Environment"
              echo "Available libraries:"
              echo "  - liboqs: Open Quantum-Safe library"
            '';
          };
        };

    in
    {
      devShells = forAllSystems makeShells;
    };
}
