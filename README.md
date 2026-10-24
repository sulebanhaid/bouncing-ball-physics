# Interactive Bouncing Ball Physics Simulation

A real-time 2D physics sandbox written in **C** using **[raylib](https://www.raylib.com/)**. Click to spawn balls, then watch them fall, bounce off the walls and collide with each other. You can change gravity and bounciness while it runs.

<!-- Demo GIF goes here: ![Demo of the simulation](demo.gif) -->

> **Download:** a ready-to-run Windows build is on the [Releases](../../releases) page.

## Features

- Add balls with a click and remove them with a right-click
- Gravity applied to every ball, adjustable while the simulation runs
- Wall collisions with adjustable energy loss ("bounce energy")
- Ball-to-ball collisions using **impulse-based collision resolution**
- Pause, clear and reset controls
- On-screen display of ball count, gravity and bounce values
- Runs at a fixed 60 FPS, with up to 100 balls at once

## Controls

| Input | Action |
|---|---|
| Left-click | Add a ball |
| Right-click | Remove the ball under the cursor |
| Space | Pause / resume |
| C | Clear all balls |
| R | Reset the simulation |
| Up / Down | Increase / decrease gravity |
| Left / Right | Decrease / increase bounce energy |

## How the physics works

Each frame, every ball's velocity is updated by gravity and its position is updated by its velocity.

**Wall collisions:** when a ball crosses a window edge, it is moved back inside and its velocity on that axis is reversed and scaled by the bounce energy (`1.0` = perfectly elastic, `0.0` = no bounce).

**Ball-to-ball collisions:** for each pair of balls:

1. **Detection:** the squared distance between centres is compared with the squared sum of the radii, which avoids a square root for pairs that aren't touching.
2. **Positional correction:** overlapping balls are pushed apart along the collision normal by half the overlap each, so they don't sink into each other.
3. **Impulse response:** the relative velocity along the normal is calculated. If the balls are moving apart, nothing happens. Otherwise an impulse of `-(1 + e) * vRel / 2` is applied in opposite directions to each ball, where `e` is the bounce energy (equal masses are assumed).

## Project structure

```
.
├── main.c    # Window setup, main loop, input handling and on-screen interface
├── ball.c    # Ball storage, movement, drawing and collision logic
└── ball.h    # Ball struct and public function declarations
```

Ball data is kept private to `ball.c` (as `static` variables), and the rest of the program interacts with it only through the functions declared in `ball.h`.

## Building from source

You need a C compiler and raylib installed.

**Windows (MinGW / w64devkit):**

```bash
gcc main.c ball.c -o game.exe -lraylib -lopengl32 -lgdi32 -lwinmm
```

**Linux:**

```bash
gcc main.c ball.c -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

Then run `./game` (or `game.exe` on Windows).

## What I learned

- Structuring a C program across multiple files, with a header acting as a public interface
- Implementing a fixed-timestep game loop and handling real-time input
- Applying vector maths (normals, dot products) to detect and resolve collisions
- Why positional correction is needed alongside velocity changes to stop objects overlapping

## Possible improvements

- Use mass proportional to ball size, so bigger balls push smaller ones further
- Use a spatial grid to speed up collision checks when there are many balls
- Use delta time so the physics speed doesn't depend on frame rate
- Build a WebAssembly version so it can be played in the browser
