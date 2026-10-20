# Level Shaders Fix — render scale alpha.7
Experimental GD 2.2081 / Geode 5.11.2 port for Windows x64, Android32 and Android64.
Includes pixelation grid correction and configurable scene render scaling.
Runtime visual/performance validation is pending; no measured FPS gain is claimed.

## Performance settings
Open this mod's Geode settings. Native scene lock and the 60 FPS target are on by default.
To experiment with lower resolution, turn native scene lock off. The saved scene
percentage defaults to 100% (native quality).
Select 75% for a first comparison, save/apply the settings; the next shader update uses the new scale. Use 50% only if the
quality tradeoff suits you. Smooth upscale enables filtering below native scale.
Restore 100% and save/apply to turn off performance scaling.
The separate pixelation correction can also be disabled.

Scaling uses the game's internal render-scale and matching sprite-composite path,
without replacing the framebuffer or changing its dimensions. The scene occupies
a smaller rendering region and is upscaled for display. At 75% per axis the nominal
scene area is 56.25%; at 50% it is 25%. These area ratios are not FPS predictions.
Full-size framebuffer memory, full-output postprocessing, game simulation,
draw-call count and object processing remain. Full-buffer clears may still cost
the same. GPU geometry fill work may fall; CPU-bound levels may see little benefit.
No automatic FPS boost or dynamic resolution is implemented. Alpha.7 includes
the separately configurable 60 FPS target described below.

Only levels already using the shader path can benefit. Shader rendering is not
forced on for other levels. Pixelation triggers and animations take priority;
scene scaling temporarily yields while pixelation is active.

## Pixelation correction
Alpha.3's whole-cell rounding and matching snap grid remain. Relative zoom and
independent X/Y values are handled. Trigger values are restored after preparation.
Invalid dimensions/zoom fall back to native behavior.

## Renderer ownership
Native framebuffer allocation, screen/target sizes, shader program, camera
transforms and context recreation remain. Settings are cached at load and updated by Geode setting-change events;
the frame path performs no settings lookup, GL query or framebuffer allocation.
Logs occur at load and once per layer for feature activation.

## Scope and testing
This is a partial port. Camera-rotation/custom-GLSL fixes and a complete GPU
shader rewrite are not implemented. GD 2.209/2.2091 remain unsupported.
See TESTING.md for controlled FPS comparisons and visual/lifecycle cases.
Alpha.1 is broken; see RECOVERY.md. Alpha.2 remains a no-hook rollback option.
Compile success does not establish runtime compatibility or an FPS gain.

Download the correct ABI from this repository's Actions run. Close GD and replace
the old package under the same mod ID. Keep the original Level Shaders Fix disabled.
Upstream attribution and pinned sources: THIRD_PARTY.md and PROVENANCE.json.

## High-object CPU diagnostics (alpha.7)
Enable CPU timing diagnostics in Geode settings; it applies live. Run a heavy level
for at least 15 seconds. Geode logs report average/max CPU duration per call for
gameplay updates, visibility, particle updates and shader calculations every five
seconds, plus the level's total loaded object count. Counts are not visible object
counts. Update timings are inclusive and categories may overlap; do not add them.
No GPU query, collision skip, trigger skip, visibility throttling or object deletion
is performed. Diagnostics have overhead; disable them for final FPS comparisons.
This instrumentation is groundwork for targeted CPU optimization, not a completed
high-object optimization or a measured FPS boost. An 8th-gen i5 does not identify the
GPU, level bottleneck or achievable performance.

## Native resolution and 60 FPS target (alpha.7)
Native scene lock and a 60 FPS target are enabled by default. Native lock overrides
the percentage slider; it means the current game framebuffer resolution, not a
forced change to the desktop display mode. GD owns resize/fullscreen handling.
Authored pixelation effects remain and may deliberately lower scene resolution.
Disable native lock to use the live percentage slider.

The frame target requests a 1/60-second render interval once the main menu exists
and after native custom-FPS updates. It changes no physics timestep, VSync setting
or saved game FPS option. Turning it off restores the previous interval and
re-applies native custom FPS settings. Pause/background handling stays native.
Other frame-control mods and OS/display limits can override pacing.
60 FPS is a target, not a measured or guaranteed minimum. Native resolution uses
more GPU work than downsampling; this cannot eliminate CPU-heavy object updates.
