# Render scale / pixelation port — alpha.7
GD 2.2081 / Geode 5.11.2. Windows x64 and Android.
Optional scene render scale: 100% native (default), 75% or 50% for less GPU scene
fill work with softer visuals. Save/apply in Geode settings; no restart is required.
Pixelation triggers take priority. Whole-cell pixelation correction is included.
Framebuffer memory and full-output shader cost remain; FPS gain is not guaranteed.
Gameplay, Android lifecycle and performance measurements remain pending.

Native scene lock and a 60 FPS target are enabled by default. Disable the lock
to use percentage scaling. Stable 60 FPS is not guaranteed on demanding levels.
