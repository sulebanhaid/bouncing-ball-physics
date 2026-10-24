# Headless renderer

This folder produces the images in `docs/` without a display or GPU.

`raylib.h` and `stub.c` are a small software stand-in for the parts of raylib the game uses. The real, unmodified `main.c` and `ball.c` are compiled against them. Mouse clicks and key presses are scripted, and chosen frames are saved to image files. The physics, the UI text and the colours all come from the game's own code. Text is drawn with raylib's default font, copied from raylib's source.

```bash
gcc -O2 -I tools/headless-render main.c ball.c tools/headless-render/stub.c -lm -o sim
SCENE=1 ./sim   # writes scene1.ppm (scenes 1-3)
GIF=1 SCENE=1 ./sim   # also writes frames to gif/ for an animation
```
