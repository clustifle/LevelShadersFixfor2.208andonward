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
