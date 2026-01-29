# Ray-Trace Renderer

<img width="1602" height="940" alt="main" src="https://github.com/user-attachments/assets/dae268a8-0bc1-4bc5-940b-2f3ad2093a0c" />

## Table of Contents
- [Description](#description)
- [Learning Objectives](#learning-objectives)
- [Screenshots](#screenshots)
- [Installation](#installation)
- [Conclusion](#conclusion)

## Description
A real-time 3D ray-trace renderer built from scratch using only the Win32 API.  
The renderer implements the Phong lighting model and supports shadows and reflections, with all rendering performed on the CPU.

## Learning Objectives
1. First steps into real-time graphics programming, including lighting models (Phong lighting), shadows, reflections, ray casting, and basic collision detection.
2. A hands-on introduction to the Win32 API and the Windows message loop, including `VirtualAlloc`, `WindowProc`, `DefWindowProc`, and `PostMessage`.
3. A deeper understanding of low-level memory management concepts such as allocation granularity, memory pages, alignment, and manual memory management.
4. Applying linear algebra to real-time applications, for example using rotation matrices to convert between camera space and world space.
5. Rendering from scratch using a software backbuffer and GDI via `StretchDIBits`.

## Screenshots

### No Lighting
<img width="1602" height="940" alt="no_light" src="https://github.com/user-attachments/assets/b120d2c3-db0d-4ce1-a862-23878b4a1a5f" />

### Lighting
<img width="1602" height="940" alt="lighting" src="https://github.com/user-attachments/assets/78b5850e-b83e-4889-9231-8477566e0a19" />

### Shadows and Reflections
<img width="1602" height="940" alt="final" src="https://github.com/user-attachments/assets/0b1ec33c-f27a-4632-866c-bc24e0df3b45" />
<img width="1602" height="940" alt="extra" src="https://github.com/user-attachments/assets/7590973c-ba9f-49e9-9e84-d2bbadfe9939" />

## Installation

## Conclusion
While there is still plenty of room for improvement—such as supporting objects beyond spheres or adding multi-threading—this project successfully achieved its goal of building a strong foundation in graphics programming and understanding the graphics pipeline.

The next steps are to:
- Implement a rasterizer
- Learn DirectX 11
- Progress to DirectX 12

<img width="1602" height="940" alt="conclusion" src="https://github.com/user-attachments/assets/449d5593-c30b-4308-8dec-dd6d1d695bd7" />
