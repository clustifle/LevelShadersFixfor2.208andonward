# Level Shaders Fix — GD 2.2081 pixelation port
Alpha.3 implements the upstream pixelation correction on GD 2.2081 / Geode 5.11.2.
Windows x64, Android32 and Android64 are build targets. Runtime validation is pending.

## Implemented
Pixelation fits a whole number of cells into the native physical render target.
Relative zoom and independent X/Y values are accounted for. Native snap-grid
preparation uses the same corrected cell size, with trigger values restored after
the call. Native animation activation, anti-aliasing and disabled-state handling
remain in place. Invalid dimensions/zoom fall back to native behavior.

The game retains ownership of framebuffer allocation, screen sizing, shader
programs, camera transforms and context recreation. No shader resources are packaged.
No FPS or VSync controls are changed.

## Scope
This is a partial port of the upstream bug fixes. Camera-rotation/custom-GLSL work
and any new performance optimization are not implemented. GD 2.2081 already uses
a rectangular target; the original square-target replacement is inappropriate.
Do not expect this build to improve FPS. See RECOVERY.md for the alpha.1 failure.

## Build and test
GitHub Actions builds on main, port-recovery and port-22081. Default target is
2.2081. GD 2.209/2.2091 remain unsupported and are rejected at compilation.
Download the matching ABI artifact from Actions; close GD and replace the previous
package under the same mod ID. Disable the original Level Shaders Fix.
See TESTING.md for required runtime checks. Alpha.2 remains a rollback option.

Upstream attribution: THIRD_PARTY.md.
