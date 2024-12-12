# engine

A simple rendering engine/game engine written in c following the tutorial of the incredible book [learnopengl](www.opengl-tutorial.org).
It is simply for learning purposes and to understand 3d rendering and the c language better.

It is build using:

- glfw
- glew
- cglm
- few stb libraries for image handeling

To run it just clone the repo and make sure you have all the dependencies installed.
For NixOs users you have to enable flakes and have direnv setup, if you want to make use of the flake system.

Then just run make and the executable:

```bash
make
./bin/main
```

The book is written for cpp use and in am translating that logic to c code. Just as a learning experience for learning c better.
