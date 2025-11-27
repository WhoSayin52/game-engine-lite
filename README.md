# Game Engine Lite / Game From Scratch

A two level top down game. Player controls a chopper and is tasked with eliminating all enemy units.

## Table of Contents
- [Screenshots](#screenshots)
- [Installation](#installation)
- [Controls](#controls)

## Screenshots

### Starting Position
<img width="828" height="666" alt="Screenshot from 2025-11-27 12-10-57" src="https://github.com/user-attachments/assets/fbc0372e-7871-49ce-b1c0-4dbcccaeec34" />
<img width="828" height="666" alt="Screenshot from 2025-11-27 12-22-24" src="https://github.com/user-attachments/assets/9bce40c9-4931-42ce-9e84-edd8b22d5bc0" />

### Full Map
<img width="1628" height="1266" alt="Screenshot from 2025-11-27 12-13-46" src="https://github.com/user-attachments/assets/063df3e0-37a4-434b-a92d-5c9d41513bdb" />
<img width="2588" height="1986" alt="Screenshot from 2025-11-27 12-21-28" src="https://github.com/user-attachments/assets/acd98169-fcce-4a81-8b6e-e4065c68e7ee" />

### Day Night Cycle
<img width="1628" height="1266" alt="Screenshot from 2025-11-27 12-14-45" src="https://github.com/user-attachments/assets/b3a3eae2-8b9d-48d7-8ae3-d708ad0e3deb" />

### Debug Mode
<img width="828" height="666" alt="Screenshot from 2025-11-27 14-37-59" src="https://github.com/user-attachments/assets/84278b46-7efd-48ba-a9bb-37d173451692" />

## Installation
### Disclaimer: game has been developed and tested on ubuntu only.

### Prequistes:
- SDL2, SDL2 images, SDL2 ttf (fonts)
- CMake

Clone repo and run:
```bash
cd game-engine-lite
cmake --preset default
cmake --build build --config Release # or Debug if desired
```
Then from shell run the game passing an argument either '1'or '2' to select a level
```bash
./path/to/exe 2 # example
```

## Controls
- Move character using arrow keys
- Fire projectile using spacebar
- Enter debug mode using f1

