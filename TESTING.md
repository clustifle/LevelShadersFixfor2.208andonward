# Alpha.3 runtime acceptance (pending)
Compare the same reproducible local level with this mod enabled and disabled.
Record GD 2.2081, Geode 5.11.2, ABI, resolution, quality, other mods and trigger values.
Disable the upstream mod. Main-menu loading alone is not validation.

- Pixelate X/Y independently and together; values 1, 2, 7, 11 and above screen size.
- Animate from 1 to larger values and back; confirm no lingering pixelation.
- Hard edges on/off; relative zoom on/off; zoom 0.5, 1 and 2.
- Snap grid on/off while scrolling, rotating and zooming the camera.
- All other shader families alone/combined; no cropping or black regions.
- Death, restart, practice checkpoints, editor playtest, pause/resume.
- Window resize, fullscreen, quality changes, 16:9 and ultrawide.
- Android background/foreground and GL context recreation.
- Compare FPS with identical settings. No FPS increase is promised.

Expected: corrected cells fill each axis without a partial cell at the edge;
snap motion follows that same cell grid. No per-frame framebuffer configuration
log messages. The activation log appears once per ShaderLayer, not every frame.

Build matrix: Windows x64, Android32, Android64. All runtime cases remain pending.
GD 2.209 and 2.2091 are unsupported.
Report level ID/local reproduction, trigger values, screenshot and latest Geode log.

## Alpha.5 scene render scale comparison
Run identical shader-active levels at 100%, 75% and 50%; save/apply each setting
change. Keep FPS cap, VSync, display refresh, graphics quality and other mods fixed.
Warm up, then repeat the same 60-second section three times for each setting.
Record average FPS and 1% lows using the same external measurement tool, plus
GPU utilization/frame time if available. A fixed frame cap can hide headroom.
Compare image quality and frame times, not just an FPS counter screenshot.
No benchmark has been run by the port author/session.

Check full-screen coverage, edges, blur, split screen, chromatic offsets and camera
rotation/zoom at every scale. Test smooth upscale on/off. Trigger pixelation on,
animate it and disable it: authored pixelation must take priority, then scene scale
must resume. Test shaders turning off, death/restart and Android context recreation.
At 100% with correction disabled, rendering should match native behavior.
Rendering work reduction is experimental; full-frame shader costs/VRAM remain.

Live settings: during a shader-active level, change 100 -> 75 -> 50 -> 100,
save/apply and resume. Confirm full-screen coverage and native sharpness at 100%.
Toggle filtering and pixelation correction too. No reload or level restart needed.
Paused rendering may retain the previous image until the next game update.

## Heavy-level diagnostics
Enable diagnostics, run the same heavy scene for 15-30 seconds at 100% then 75%,
and collect the 'LSF CPU timings' lines and actual FPS separately.
Disable diagnostics for final performance comparisons. CPU timings exclude GPU
execution and are inclusive, so categories cannot be summed into a frame total.
Compare trigger timing, collisions and object visibility with diagnostics off/on.
